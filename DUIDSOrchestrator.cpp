// DUIDSOrchestrator.cpp
// Concrete implementation of the IDUIDSOrchestratorService.

#include "HexademicSixLattice.h" // Includes Hexademic core types and interfaces
#include "Logging/LogMacros.h"   // For UE_LOG
#include "HAL/PlatformTime.h"    // For FPlatformTime::Seconds()
#include "Misc/Guid.h"           // For FGuid
#include "Math/UnrealMathUtility.h" // For FMath::RandRange

// Define a log category for Hexademic Lattice operations (if not already defined in Hexademic6Module.cpp)
// DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);

FDUIDSOrchestrator::FDUIDSOrchestrator()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("FDUIDSOrchestrator constructed."));
}

FDUIDSOrchestrator::~FDUIDSOrchestrator()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("FDUIDSOrchestrator destructed."));
}

FDUIDSIndex FDUIDSOrchestrator::GenerateIndex(const FHexademicMemoryNode& Memory)
{
    // Generates a DUIDS index for a given memory node.
    // This calls the inlined UpdateDUIDSIndex on the memory's LatticePosition
    // and then uses the result.
    FDUIDSIndex NewIndex = Memory.LatticePosition.DUIDSLocation;
    // Potentially add more unique identifiers based on EventType or EventData hash
    // NewIndex.Cutter = FCRC::MemCrc32(Memory.EventType.GetCharArray().GetData(), Memory.EventType.Len() * sizeof(TCHAR)) % 65536;
    
    IndexToMemoryMap.Add(NewIndex, Memory.MemoryID);
    MemoryToIndexMap.Add(Memory.MemoryID, NewIndex);

    UE_LOG(LogHexademicLattice, Verbose, TEXT("Generated DUIDS Index %s for Memory %s."), *NewIndex.ToDecimalString(), *Memory.MemoryID.ToString());
    return NewIndex;
}

TOptional<FHexademicMemoryNode> FDUIDSOrchestrator::RetrieveByIndex(const FDUIDSIndex& Index, bool bDecompress)
{
    // Retrieves a memory node using its DUIDS index.
    if (FGuid* MemoryIDPtr = IndexToMemoryMap.Find(Index))
    {
        if (TArray<uint8>* CompressedData = CompressedMemoryStorage.Find(Index))
        {
            FHexademicMemoryNode RetrievedMemory = DecompressMemoryData(*CompressedData);
            RetrievedMemory.QuickAccessIndex = Index; // Restore quick access index
            
            if (bDecompress)
            {
                RetrievedMemory.DecompressForAccess(); // Call inlined method
            }
            TrackMemoryAccess(Index); // Track access
            UE_LOG(LogHexademicLattice, Log, TEXT("Retrieved Memory %s by DUIDS Index %s. Decompressed: %s"), *RetrievedMemory.MemoryID.ToString(), *Index.ToDecimalString(), bDecompress ? TEXT("True") : TEXT("False"));
            return RetrievedMemory;
        }
    }
    UE_LOG(LogHexademicLattice, Warning, TEXT("Memory not found for DUIDS Index %s."), *Index.ToDecimalString());
    return TOptional<FHexademicMemoryNode>();
}

TArray<FDUIDSIndex> FDUIDSOrchestrator::QueryRange(const FDUIDSIndex& StartIndex, const FDUIDSIndex& EndIndex)
{
    // Queries for all DUIDS indices within a specified range.
    // This assumes FDUIDSIndex implements operator< for sorting.
    TArray<FDUIDSIndex> ResultIndices;
    TArray<FDUIDSIndex> AllIndices;
    IndexToMemoryMap.GetKeys(AllIndices);
    AllIndices.Sort(); // Sort to allow range querying

    for (const FDUIDSIndex& Index : AllIndices)
    {
        if (Index < StartIndex) continue;
        if (EndIndex < Index) break; // Optimization for sorted list
        ResultIndices.Add(Index);
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Queried %d DUIDS indices between %s and %s."), ResultIndices.Num(), *StartIndex.ToDecimalString(), *EndIndex.ToDecimalString());
    return ResultIndices;
}

void FDUIDSOrchestrator::CompressMemoryNode(FHexademicMemoryNode& Memory, uint8 CompressionLevel)
{
    // Compresses a memory node and stores its compressed data.
    Memory.CompressForStorage(); // Calls the inlined method
    TArray<uint8> CompressedData = CompressMemoryData(Memory, CompressionLevel);
    CompressedMemoryStorage.Add(Memory.QuickAccessIndex, CompressedData);
    UE_LOG(LogHexademicLattice, Log, TEXT("Compressed Memory %s to level %d. Stored %d bytes."), *Memory.MemoryID.ToString(), CompressionLevel, CompressedData.Num());
}

void FDUIDSOrchestrator::DecompressMemoryNode(FHexademicMemoryNode& Memory)
{
    // Decompresses a memory node.
    Memory.DecompressForAccess(); // Calls the inlined method
    if (TArray<uint8>* CompressedData = CompressedMemoryStorage.Find(Memory.QuickAccessIndex))
    {
        FHexademicMemoryNode Decompressed = DecompressMemoryData(*CompressedData);
        Memory.EventData = Decompressed.EventData; // Restore original data
        Memory.EmotionalColor = Decompressed.EmotionalColor;
        // ... restore other fields as needed based on compression
        UE_LOG(LogHexademicLattice, Log, TEXT("Decompressed Memory %s."), *Memory.MemoryID.ToString());
    }
}

float FDUIDSOrchestrator::GetCompressionRatio(ECognitiveLatticeOrder Order) const
{
    // Placeholder: Returns the average compression ratio for memories in a given order.
    // This would require tracking original vs. compressed sizes for each memory.
    UE_LOG(LogHexademicLattice, Log, TEXT("Getting compression ratio for Order %d. (Placeholder)"), (uint8)Order);
    // For demonstration, return a fixed value
    return 0.75f; // 75% compression
}

TOptional<float> FDUIDSOrchestrator::GetMemoryResonance(const FDUIDSIndex& Index)
{
    // Retrieves memory resonance directly from cache or by partial decompression.
    if (float* Resonance = ResonanceCache.Find(Index))
    {
        return *Resonance;
    }
    // Placeholder for partial decompression or fetching from the main memory storage
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Getting memory resonance for %s. (Not in cache, requires partial fetch)"), *Index.ToDecimalString());
    if (TOptional<FHexademicMemoryNode> Memory = RetrieveByIndex(Index, false)) // Retrieve without full decompression
    {
        UpdateCachesForMemory(Index, Memory.GetValue()); // Update cache for future access
        return Memory->ResonanceStrength;
    }
    return TOptional<float>();
}

TOptional<FVector> FDUIDSOrchestrator::GetEmotionalSignature(const FDUIDSIndex& Index)
{
    // Retrieves emotional signature directly from cache or by partial decompression.
    if (FVector* Signature = EmotionalSignatureCache.Find(Index))
    {
        return *Signature;
    }
    // Placeholder for partial decompression or fetching from the main memory storage
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Getting emotional signature for %s. (Not in cache, requires partial fetch)"), *Index.ToDecimalString());
    if (TOptional<FHexademicMemoryNode> Memory = RetrieveByIndex(Index, false))
    {
        UpdateCachesForMemory(Index, Memory.GetValue());
        return FVector(Memory->EmotionalValence, Memory->EmotionalIntensity, Memory->MythicDepth); // Example vector
    }
    return TOptional<FVector>();
}

TArray<FDUIDSIndex> FDUIDSOrchestrator::GetCrossReferences(const FDUIDSIndex& Index)
{
    // Retrieves cross-references for a memory, potentially without full decompression.
    UE_LOG(LogHexademicLattice, Log, TEXT("Getting cross-references for %s. (Requires partial fetch)"), *Index.ToDecimalString());
    if (TOptional<FHexademicMemoryNode> Memory = RetrieveByIndex(Index, false))
    {
        return Memory->CrossReferences;
    }
    return TArray<FDUIDSIndex>();
}

void FDUIDSOrchestrator::TrackMemoryAccess(const FDUIDSIndex& Index)
{
    // Tracks when a memory is accessed, updating counts and timestamps.
    AccessCounts.FindOrAdd(Index)++;
    LastAccessTimes.Add(Index, FPlatformTime::Seconds());
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Tracked access for DUIDS Index %s. Count: %d"), *Index.ToDecimalString(), AccessCounts[Index]);
}

TArray<FDUIDSIndex> FDUIDSOrchestrator::GetMostAccessed(int32 Count, ECognitiveLatticeOrder Order)
{
    // Returns the DUIDS indices of the most frequently accessed memories within a given order.
    TArray<FDUIDSIndex> MostAccessedIndices;
    TArray<FDUIDSIndex> AllIndices;
    AccessCounts.GetKeys(AllIndices);

    // Filter by order (assuming DUIDSIndex can be mapped to an order, or memory itself can)
    // For simplicity, this example just takes from all accessed, then sorts.
    AllIndices.Sort([this](const FDUIDSIndex& A, const FDUIDSIndex& B)
    {
        return AccessCounts.FindOrAdd(A) > AccessCounts.FindOrAdd(B); // Sort descending by count
    });

    for (int32 i = 0; i < FMath::Min(Count, AllIndices.Num()); ++i)
    {
        MostAccessedIndices.Add(AllIndices[i]);
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Retrieved %d most accessed DUIDS indices for Order %d."), MostAccessedIndices.Num(), (uint8)Order);
    return MostAccessedIndices;
}

TMap<FDUIDSIndex, int32> FDUIDSOrchestrator::GetAccessPatterns(float TimeWindow)
{
    // Returns access patterns (index to count) within a specified time window.
    TMap<FDUIDSIndex, int32> Patterns;
    double CurrentTime = FPlatformTime::Seconds();
    for (const auto& Pair : LastAccessTimes)
    {
        if ((CurrentTime - Pair.Value) <= TimeWindow)
        {
            Patterns.Add(Pair.Key, AccessCounts.FindOrAdd(Pair.Key));
        }
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Retrieved %d access patterns within %f seconds."), Patterns.Num(), TimeWindow);
    return Patterns;
}

void FDUIDSOrchestrator::OptimizeIndices(ECognitiveLatticeOrder Order)
{
    // Placeholder: Optimizes the internal index structure for a given order.
    // This could involve re-balancing, defragmentation, or re-clustering.
    UE_LOG(LogHexademicLattice, Log, TEXT("Optimizing DUIDS indices for Order %d. (Placeholder)"), (uint8)Order);
}

void FDUIDSOrchestrator::RebuildIndexForOrder(ECognitiveLatticeOrder Order)
{
    // Placeholder: Rebuilds the DUIDS index entirely for a specific lattice order.
    // This is a costly operation typically done during maintenance.
    UE_LOG(LogHexademicLattice, Log, TEXT("Rebuilding DUIDS index for Order %d. (Placeholder)"), (uint8)Order);
    // You'd iterate through all memories in that order and regenerate/re-add their DUIDS indices.
}

float FDUIDSOrchestrator::GetIndexFragmentation(ECognitiveLatticeOrder Order) const
{
    // Placeholder: Calculates a metric for the fragmentation of the DUIDS index.
    // Lower fragmentation means more efficient sequential access.
    UE_LOG(LogHexademicLattice, Log, TEXT("Getting DUIDS index fragmentation for Order %d. (Placeholder)"), (uint8)Order);
    // For demonstration, return a fixed value
    return FMath::RandRange(0.0f, 1.0f);
}

// =============================================================================
// Internal Helper Methods
// =============================================================================

FDUIDSIndex FDUIDSOrchestrator::GenerateIndexFromCoordinate(const FHexademic6DCoordinate& Coord, ECognitiveLatticeOrder Order)
{
    // This helper maps directly to FHexademic6DCoordinate's UpdateDUIDSIndex,
    // ensuring consistency. The DUIDSLocation should already be set on the Coord.
    FDUIDSIndex Index = Coord.DUIDSLocation;
    return Index;
}

void FDUIDSOrchestrator::UpdateCachesForMemory(const FDUIDSIndex& Index, const FHexademicMemoryNode& Memory)
{
    // Populates sliced data caches for fast retrieval.
    ResonanceCache.Add(Index, Memory.ResonanceStrength);
    EmotionalSignatureCache.Add(Index, FVector(Memory.EmotionalValence, Memory.EmotionalIntensity, Memory.MythicDepth));
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Updated caches for DUIDS Index %s."), *Index.ToDecimalString());
}

void FDUIDSOrchestrator::InvalidateCachesForIndex(const FDUIDSIndex& Index)
{
    // Invalidates cached sliced data for a specific index, typically after modification.
    ResonanceCache.Remove(Index);
    EmotionalSignatureCache.Remove(Index);
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Invalidated caches for DUIDS Index %s."), *Index.ToDecimalString());
}

TArray<uint8> FDUIDSOrchestrator::CompressMemoryData(const FHexademicMemoryNode& Memory, uint8 Level)
{
    // Placeholder: Compresses the actual EventData and other fields into a byte array.
    // This would use a compression library (e.g., Zlib, LZ4, or custom).
    UE_LOG(LogHexademicLattice, Log, TEXT("Compressing memory data for %s at level %d. (Placeholder)"), *Memory.MemoryID.ToString(), Level);
    TArray<uint8> CompressedBytes;
    FString DataToCompress = Memory.EventData + Memory.EventType; // Example data
    
    // Simulate compression: just copy some bytes or a dummy value
    CompressedBytes.Add(Level); // Store compression level as first byte
    for (int32 i = 0; i < FMath::Min(DataToCompress.Len(), 10); ++i)
    {
        CompressedBytes.Add((uint8)DataToCompress[i]);
    }
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Compressed %d bytes (simulated)."), CompressedBytes.Num());
    return CompressedBytes;
}

FHexademicMemoryNode FDUIDSOrchestrator::DecompressMemoryData(const TArray<uint8>& CompressedData)
{
    // Placeholder: Decompresses a byte array back into an FHexademicMemoryNode.
    UE_LOG(LogHexademicLattice, Log, TEXT("Decompressing memory data from %d bytes. (Placeholder)"), CompressedData.Num());
    FHexademicMemoryNode DecompressedMemory;
    if (CompressedData.Num() > 0)
    {
        DecompressedMemory.CompressionLevel = CompressedData[0]; // Recover level
        DecompressedMemory.EventData = TEXT("Decompressed Data (Simulated)");
        DecompressedMemory.EventType = TEXT("Decompressed Event");
        // ... restore other fields
    }
    return DecompressedMemory;
}
