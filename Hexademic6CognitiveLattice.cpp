// Hexademic6CognitiveLattice.cpp
// This file implements the concrete FHexademic6CognitiveLattice class.

#include "HexademicSixLattice.h"
#include "Misc/Guid.h"
#include "Math/UnrealMathUtility.h"
#include "HAL/PlatformTime.h"
#include "Logging/LogMacros.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "CoreMinimal.h" // For UE_LOG and other core types
#include "Misc/Optional.h" // For TOptional

// Define a log category for Hexademic Lattice operations
// (This is defined in HexademicSixLattice.cpp as well; ensure no redefinition issues in build system)
// DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);

// Forward declarations for placeholder services
// In a full implementation, these would be proper concrete classes defined elsewhere.
class FHexademic6ResonanceServicePlaceholder : public IHexademic6ResonanceService
{
public:
    virtual void UpdateResonanceField(const TArray<FHexademicMemoryNode>& ActiveMemories) override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: UpdateResonanceField called.")); }
    virtual float SampleResonanceAt(const FHexademic6DCoordinate& Position) const override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: SampleResonanceAt called.")); return 0.5f; }
    // Omitting GetResonanceGradient due to FVector6 being an undefined type in this context.
    // virtual FVector6 GetResonanceGradient(const FHexademic6DCoordinate& Position) const override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: GetResonanceGradient called.")); return FVector6(); }
    virtual float CalculateCrossDimensionalResonance() const override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: CalculateCrossDimensionalResonance called.")); return 0.0f; }
    virtual TArray<FHexademic6DCoordinate> GetResonanceHotspots(ECognitiveLatticeOrder Order) const override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: GetResonanceHotspots called.")); return TArray<FHexademic6DCoordinate>(); }
    virtual void PropagateResonanceWave(const FHexademic6DCoordinate& Origin, float Amplitude) override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: PropagateResonanceWave called.")); }
    virtual float GetGlobalCoherence() const override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: GetGlobalCoherence called.")); return 0.0f; }
    virtual float GetOrderCoherence(ECognitiveLatticeOrder Order) const override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: GetOrderCoherence called.")); return 0.0f; }
    virtual void SubscribeToCoherenceUpdates(TFunction<void(float)> Callback) override { UE_LOG(LogHexademicLattice, Log, TEXT("ResonanceService Placeholder: SubscribeToCoherenceUpdates called.")); }
};

class FHexademic6MythicServicePlaceholder : public IHexademic6MythicService
{
public:
    virtual void ProcessMythicEmergence(const TArray<FHexademicMemoryNode>& DeepMemories) override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: ProcessMythicEmergence called.")); }
    virtual TArray<FString> ExtractNarrativeThreads(ECognitiveLatticeOrder MinOrder = ECognitiveLatticeOrder::Order72) override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: ExtractNarrativeThreads called.")); return TArray<FString>(); }
    virtual void RecordCollectiveResonance(const FHexademic6DCoordinate& MythicCenter, float Intensity) override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: RecordCollectiveResonance called.")); }
    virtual void UpdateArchetypeActivations(const TMap<uint32, float>& Activations) override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: UpdateArchetypeActivations called.")); }
    virtual TArray<uint32> GetActiveArchetypes(float MinActivation = 0.5f) const override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: GetActiveArchetypes called.")); return TArray<uint32>(); }
    virtual float GetArchetypeResonance(uint32 ArchetypeID) const override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: GetArchetypeResonance called.")); return 0.0f; }
    virtual void TriggerTranscendentExperience(const FHexademic6DCoordinate& FocalPoint) override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: TriggerTranscendentExperience called.")); }
    virtual bool IsInTranscendentState() const override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: IsInTranscendentState called.")); return false; }
    virtual float GetTranscendenceLevel() const override { UE_LOG(LogHexademicLattice, Log, TEXT("MythicService Placeholder: GetTranscendenceLevel called.")); return 0.0f; }
};

// =============================================================================
// FHexademic6CognitiveLattice Implementation
// =============================================================================

FHexademic6CognitiveLattice::FHexademic6CognitiveLattice()
{
    InitializeOrderCapacities();
    UE_LOG(LogHexademicLattice, Log, TEXT("FHexademic6CognitiveLattice constructed."));
}

FHexademic6CognitiveLattice::~FHexademic6CognitiveLattice()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("FHexademic6CognitiveLattice destructed."));
}

void FHexademic6CognitiveLattice::InitializeOrderCapacities()
{
    // Define the maximum number of memories each lattice order can hold.
    // These are example values, potentially based on N^6 where N is OrderSize.
    OrderCapacities.Add(ECognitiveLatticeOrder::Order12, 12 * 12 * 12 * 12 * 12 * 12);
    OrderCapacities.Add(ECognitiveLatticeOrder::Order18, 18 * 18 * 18 * 18 * 18 * 18);
    OrderCapacities.Add(ECognitiveLatticeOrder::Order36, 36 * 36 * 36 * 36 * 36 * 36);
    OrderCapacities.Add(ECognitiveLatticeOrder::Order72, 72 * 72 * 72 * 72 * 72 * 72);
    OrderCapacities.Add(ECognitiveLatticeOrder::Order144, 144 * 144 * 144 * 144 * 144 * 144);
    OrderCapacities.Add(ECognitiveLatticeOrder::OrderInfinite, TNumericLimits<int32>::Max()); // Represents effectively unbounded
}

void FHexademic6CognitiveLattice::StoreMemory(const FHexademicMemoryNode& Memory)
{
    // Stores a memory node within the appropriate lattice order.
    TArray<FHexademicMemoryNode>& MemoriesInOrder = LatticeOrders.FindOrAdd(Memory.LatticePosition.LatticeOrder);
    // Check capacity if needed: if (MemoriesInOrder.Num() < OrderCapacities[Memory.LatticePosition.LatticeOrder])
    MemoriesInOrder.Add(Memory);
    MemoryToOrderMap.Add(Memory.MemoryID, Memory.LatticePosition.LatticeOrder);
    UpdateResonanceConnections(Memory.MemoryID);
    UE_LOG(LogHexademicLattice, Log, TEXT("Stored memory: %s in Order %d."), *Memory.MemoryID.ToString(), (uint8)Memory.LatticePosition.LatticeOrder);
}

TOptional<FHexademicMemoryNode> FHexademic6CognitiveLattice::RetrieveMemory(const FGuid& MemoryID)
{
    // Retrieves a memory node by its unique ID.
    if (ECognitiveLatticeOrder* OrderPtr = MemoryToOrderMap.Find(MemoryID))
    {
        if (TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(*OrderPtr))
        {
            for (FHexademicMemoryNode& Memory : *MemoriesInOrder)
            {
                if (Memory.MemoryID == MemoryID)
                {
                    Memory.AccessCount++; // Increment access count on retrieval
                    UE_LOG(LogHexademicLattice, Log, TEXT("Retrieved memory: %s from Order %d."), *MemoryID.ToString(), (uint8)*OrderPtr);
                    return Memory;
                }
            }
        }
    }
    UE_LOG(LogHexademicLattice, Warning, TEXT("Memory %s not found."), *MemoryID.ToString());
    return TOptional<FHexademicMemoryNode>();
}

void FHexademic6CognitiveLattice::UpdateMemoryResonance(const FGuid& MemoryID, float NewResonance)
{
    // Updates the resonance strength of a specific memory.
    if (ECognitiveLatticeOrder* OrderPtr = MemoryToOrderMap.Find(MemoryID))
    {
        if (TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(*OrderPtr))
        {
            for (FHexademicMemoryNode& Memory : *MemoriesInOrder)
            {
                if (Memory.MemoryID == MemoryID)
                {
                    Memory.ResonanceStrength = FMath::Clamp(NewResonance, 0.0f, 1.0f);
                    UE_LOG(LogHexademicLattice, Log, TEXT("Updated resonance for memory %s to %f."), *MemoryID.ToString(), NewResonance);
                    return;
                }
            }
        }
    }
    UE_LOG(LogHexademicLattice, Warning, TEXT("Could not update resonance for memory %s: not found."), *MemoryID.ToString());
}

void FHexademic6CognitiveLattice::PromoteMemoryToOrder(const FGuid& MemoryID, ECognitiveLatticeOrder NewOrder)
{
    // Migrates a memory from its current order to a new, specified order.
    if (ECognitiveLatticeOrder* CurrentOrderPtr = MemoryToOrderMap.Find(MemoryID))
    {
        ECognitiveLatticeOrder CurrentOrder = *CurrentOrderPtr;
        if (CurrentOrder != NewOrder)
        {
            MigrateMemoryBetweenOrders(MemoryID, CurrentOrder, NewOrder);
            UE_LOG(LogHexademicLattice, Log, TEXT("Promoted memory %s from Order %d to Order %d."), *MemoryID.ToString(), (uint8)CurrentOrder, (uint8)NewOrder);
        }
        else
        {
            UE_LOG(LogHexademicLattice, Warning, TEXT("Memory %s is already in target Order %d."), *MemoryID.ToString(), (uint8)NewOrder);
        }
    }
    else
    {
        UE_LOG(LogHexademicLattice, Warning, TEXT("Cannot promote memory %s: not found."), *MemoryID.ToString());
    }
}

TArray<FHexademicMemoryNode> FHexademic6CognitiveLattice::GetMemoriesInOrder(ECognitiveLatticeOrder Order) const
{
    // Returns all memories currently residing within a specified cognitive lattice order.
    if (const TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(Order))
    {
        UE_LOG(LogHexademicLattice, Log, TEXT("Retrieved %d memories from Order %d."), MemoriesInOrder->Num(), (uint8)Order);
        return *MemoriesInOrder;
    }
    UE_LOG(LogHexademicLattice, Warning, TEXT("No memories found in Order %d."), (uint8)Order);
    return TArray<FHexademicMemoryNode>();
}

TArray<FHexademicMemoryNode> FHexademic6CognitiveLattice::GetMemoriesNearCoordinate(const FHexademic6DCoordinate& Center, float Radius) const
{
    // Finds memories within a certain 'radius' (resonance threshold) of a given 6D coordinate.
    TArray<FHexademicMemoryNode> ResultMemories;
    if (const TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(Center.LatticeOrder))
    {
        for (const FHexademicMemoryNode& Memory : *MemoriesInOrder)
        {
            if (Memory.LatticePosition.CalculateResonanceWith(Center) >= (1.0f - Radius)) // Higher resonance = closer
            {
                ResultMemories.Add(Memory);
            }
        }
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Found %d memories near coordinate in Order %d with radius %f."), ResultMemories.Num(), (uint8)Center.LatticeOrder, Radius);
    return ResultMemories;
}

TArray<FHexademicMemoryNode> FHexademic6CognitiveLattice::GetResonantMemories(const FGuid& SourceMemoryID, float MinResonance) const
{
    // Retrieves memories that are linked and resonate with a source memory.
    TArray<FHexademicMemoryNode> ResonantMemories;
    if (const TArray<FGuid>* Resonances = ResonanceGraph.Find(SourceMemoryID))
    {
        // Note: const_cast is used here to allow RetrieveMemory to increment AccessCount.
        // In a true const method, a separate non-mutating retrieval would be preferred.
        for (const FGuid& ResonantMemoryID : *Resonances)
        {
            if (TOptional<FHexademicMemoryNode> Memory = const_cast<FHexademic6CognitiveLattice*>(this)->RetrieveMemory(ResonantMemoryID))
            {
                if (Memory->ResonanceStrength >= MinResonance) 
                {
                     ResonantMemories.Add(Memory.GetValue());
                }
            }
        }
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Found %d resonant memories for %s with min resonance %f."), ResonantMemories.Num(), *SourceMemoryID.ToString(), MinResonance);
    return ResonantMemories;
}

TArray<FHexademic6DCoordinate> FHexademic6CognitiveLattice::DetectEmergentPatterns(ECognitiveLatticeOrder Order) const
{
    // Placeholder: This method would implement complex pattern detection algorithms.
    // Examples include spatial clustering, temporal correlations, or archetypal signature matching.
    UE_LOG(LogHexademicLattice, Log, TEXT("Detecting emergent patterns in Order %d... (Placeholder)"), (uint8)Order);
    TArray<FHexademic6DCoordinate> EmergentPatterns;
    // Example: For demonstration, just return the coordinates of the first few memories.
    if (const TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(Order))
    {
        for (int32 i = 0; i < FMath::Min(MemoriesInOrder->Num(), 3); ++i)
        {
            EmergentPatterns.Add(MemoriesInOrder->operator[](i).LatticePosition);
        }
    }
    return EmergentPatterns;
}

float FHexademic6CognitiveLattice::CalculateOrderCoherence(ECognitiveLatticeOrder Order) const
{
    // Placeholder: Calculates a coherence metric for memories within a given order.
    // This could involve average resonance, density, or structural integrity.
    if (const TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(Order))
    {
        if (MemoriesInOrder->Num() == 0) return 0.0f;

        float TotalResonance = 0.0f;
        for (const FHexademicMemoryNode& Memory : *MemoriesInOrder)
        {
            TotalResonance += Memory.ResonanceStrength;
        }
        float Coherence = TotalResonance / MemoriesInOrder->Num();
        UE_LOG(LogHexademicLattice, Log, TEXT("Calculated coherence for Order %d: %f."), (uint8)Order, Coherence);
        return Coherence;
    }
    return 0.0f;
}

TArray<uint32> FHexademic6CognitiveLattice::GetMostActiveArchetypes(ECognitiveLatticeOrder Order) const
{
    // Identifies the archetypes most frequently associated with memories in a given order.
    TMap<uint32, int32> ArchetypeCounts;
    if (const TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(Order))
    {
        for (const FHexademicMemoryNode& Memory : *MemoriesInOrder)
        {
            for (uint32 ArchetypeID : Memory.AssociatedArchetypes)
            {
                ArchetypeCounts.FindOrAdd(ArchetypeID)++;
            }
        }
    }

    TArray<uint32> ActiveArchetypes;
    // Simple logic: return archetypes that appear more than once
    for (auto const& Pair : ArchetypeCounts)
    {
        if (Pair.Value > 1)
        {
            ActiveArchetypes.Add(Pair.Key);
        }
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Found %d active archetypes in Order %d."), ActiveArchetypes.Num(), (uint8)Order);
    return ActiveArchetypes;
}

void FHexademic6CognitiveLattice::TriggerLatticeResonance()
{
    // Initiates a system-wide resonance event. This would typically interact with a resonance service.
    UE_LOG(LogHexademicLattice, Log, TEXT("Triggering global lattice resonance... (Placeholder)"));
    // Example: Call placeholder resonance service if registered
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        FHexademic6ServiceLocator::GetResonanceService().UpdateResonanceField(TArray<FHexademicMemoryNode>()); // Pass relevant memories
    }
}

void FHexademic6CognitiveLattice::ProcessMemoryMigration()
{
    // Iterates through all memories, evaluating if they should be promoted or demoted
    // to different lattice orders based on their properties and then performs the migration.
    UE_LOG(LogHexademicLattice, Log, TEXT("Processing memory migration across orders..."));
    TArray<TPair<FGuid, ECognitiveLatticeOrder>> MigrationsToPerform;

    for (auto& Pair : LatticeOrders)
    {
        ECognitiveLatticeOrder CurrentOrder = Pair.Key;
        TArray<FHexademicMemoryNode>& Memories = Pair.Value;

        // Iterate backwards to safely remove elements during iteration
        for (int32 i = Memories.Num() - 1; i >= 0; --i)
        {
            FHexademicMemoryNode& Memory = Memories[i];
            ECognitiveLatticeOrder OptimalOrder = Memory.DetermineOptimalOrder();

            if (OptimalOrder != CurrentOrder)
            {
                MigrationsToPerform.Add(TPair<FGuid, ECognitiveLatticeOrder>(Memory.MemoryID, OptimalOrder));
                // Remove from current list, will be re-added to target order by PromoteMemoryToOrder
                Memories.RemoveAt(i); 
            }
        }
    }

    // Execute all collected migrations
    for (const TPair<FGuid, ECognitiveLatticeOrder>& Migration : MigrationsToPerform)
    {
        PromoteMemoryToOrder(Migration.Key, Migration.Value);
    }
}

void FHexademic6CognitiveLattice::UpdateTemporalDecay(float DeltaTime)
{
    // Applies a decay factor to memories over time, affecting their temporal persistence and resonance.
    UE_LOG(LogHexademicLattice, Log, TEXT("Updating temporal decay for memories with DeltaTime: %f."), DeltaTime);
    for (auto& Pair : LatticeOrders)
    {
        for (FHexademicMemoryNode& Memory : Pair.Value)
        {
            Memory.TemporalDecay = FMath::Min(1.0f, Memory.TemporalDecay + (DeltaTime * 0.01f)); // Example decay rate
            Memory.ResonanceStrength = FMath::Max(0.0f, Memory.ResonanceStrength - (DeltaTime * 0.005f)); // Example resonance decay
        }
    }
}

void FHexademic6CognitiveLattice::EvolveConsciousness()
{
    // Triggers a consciousness evolution process, which could involve re-evaluating
    // lattice structures, optimizing connections, or pruning dormant nodes.
    UE_LOG(LogHexademicLattice, Log, TEXT("Evolving consciousness... (Placeholder)"));
    for (auto& Pair : LatticeOrders)
    {
        PerformOrderEvolution(Pair.Key);
    }
}

float FHexademic6CognitiveLattice::GetLatticeComplexity() const
{
    // Calculates an overall complexity metric for the entire cognitive lattice.
    // This is a simple metric combining total memories and resonance graph connections.
    int32 TotalMemories = 0;
    for (const auto& Pair : LatticeOrders)
    {
        TotalMemories += Pair.Value.Num();
    }
    float Complexity = (float)TotalMemories * 0.1f + ResonanceGraph.Num() * 0.01f;
    UE_LOG(LogHexademicLattice, Log, TEXT("Calculated lattice complexity: %f."), Complexity);
    return Complexity;
}

TMap<ECognitiveLatticeOrder, int32> FHexademic6CognitiveLattice::GetOrderPopulations() const
{
    // Returns a map showing the number of memories in each cognitive lattice order.
    TMap<ECognitiveLatticeOrder, int32> Populations;
    for (const auto& Pair : LatticeOrders)
    {
        Populations.Add(Pair.Key, Pair.Value.Num());
    }
    UE_LOG(LogHexademicLattice, Log, TEXT("Retrieved order populations."));
    return Populations;
}

void FHexademic6CognitiveLattice::MigrateMemoryBetweenOrders(const FGuid& MemoryID, ECognitiveLatticeOrder FromOrder, ECognitiveLatticeOrder ToOrder)
{
    // Internal helper function to move a memory node from one order to another.
    if (TArray<FHexademicMemoryNode>* SourceMemories = LatticeOrders.Find(FromOrder))
    {
        int32 FoundIndex = INDEX_NONE;
        FHexademicMemoryNode MigratingMemory;
        for (int32 i = 0; i < SourceMemories->Num(); ++i)
        {
            if (SourceMemories->operator[](i).MemoryID == MemoryID)
            {
                MigratingMemory = SourceMemories->operator[](i);
                FoundIndex = i;
                break;
            }
        }

        if (FoundIndex != INDEX_NONE)
        {
            SourceMemories->RemoveAt(FoundIndex);
            
            // Update the memory's internal lattice order property
            MigratingMemory.LatticePosition.LatticeOrder = ToOrder;

            // Add to target order's memory list
            TArray<FHexademicMemoryNode>& TargetMemories = LatticeOrders.FindOrAdd(ToOrder);
            TargetMemories.Add(MigratingMemory);
            MemoryToOrderMap.Add(MemoryID, ToOrder); // Update the quick lookup map
            UE_LOG(LogHexademicLattice, Log, TEXT("Memory %s migrated internally from Order %d to Order %d."), *MemoryID.ToString(), (uint8)FromOrder, (uint8)ToOrder);
        }
        else
        {
            UE_LOG(LogHexademicLattice, Warning, TEXT("Memory %s not found in source Order %d for migration."), *MemoryID.ToString(), (uint8)FromOrder);
        }
    }
    else
    {
        UE_LOG(LogHexademicLattice, Warning, TEXT("Source Order %d not found for memory migration."), (uint8)FromOrder);
    }
}

void FHexademic6CognitiveLattice::UpdateResonanceConnections(const FGuid& MemoryID)
{
    // Placeholder: Updates the internal resonance graph for a given memory.
    // This would involve finding nearby or related memories and establishing bidirectional links.
    UE_LOG(LogHexademicLattice, Log, TEXT("Updating resonance connections for memory %s (Placeholder)"), *MemoryID.ToString());
    // Example: Connect to a few random memories in the same order
    if (ECognitiveLatticeOrder* OrderPtr = MemoryToOrderMap.Find(MemoryID))
    {
        if (TArray<FHexademicMemoryNode>* MemoriesInOrder = LatticeOrders.Find(*OrderPtr))
        {
            TArray<FGuid>& ConnectedMemories = ResonanceGraph.FindOrAdd(MemoryID);
            for (int i = 0; i < FMath::Min(MemoriesInOrder->Num(), 5); ++i) // Connect to up to 5 memories
            {
                if (MemoriesInOrder->IsValidIndex(i) && MemoriesInOrder->operator[](i).MemoryID != MemoryID)
                {
                    ConnectedMemories.AddUnique(MemoriesInOrder->operator[](i).MemoryID);
                    // Also establish bidirectional link
                    ResonanceGraph.FindOrAdd(MemoriesInOrder->operator[](i).MemoryID).AddUnique(MemoryID);
                }
            }
        }
    }
}

void FHexademic6CognitiveLattice::PerformOrderEvolution(ECognitiveLatticeOrder Order)
{
    // Placeholder: Implements evolutionary processes specific to a lattice order.
    // This could involve re-balancing memories, optimizing connections, or pruning dormant nodes.
    UE_LOG(LogHexademicLattice, Log, TEXT("Performing evolution for Order %d... (Placeholder)"), (uint8)Order);
    // Example: remove memories that should decay out of this order
    if (TArray<FHexademicMemoryNode>* Memories = LatticeOrders.Find(Order))
    {
        for (int32 i = Memories->Num() - 1; i >= 0; --i)
        {
            if (ShouldDemoteMemory(Memories->operator[](i)))
            {
                UE_LOG(LogHexademicLattice, Log, TEXT("Memory %s decaying out of Order %d during evolution."), *Memories->operator[](i).MemoryID.ToString(), (uint8)Order);
                // In a real system, you might migrate it to a lower order instead of just removing.
                Memories->RemoveAt(i);
                MemoryToOrderMap.Remove(Memories->operator[](i).MemoryID); // Update map
            }
        }
    }
}

float FHexademic6CognitiveLattice::CalculateMemoryComplexity(const FHexademicMemoryNode& Memory) const
{
    // Placeholder: Calculates a complexity score for a single memory.
    // This could be based on event data length, number of associated archetypes, or access patterns.
    return (float)Memory.EventData.Len() * 0.01f + Memory.AssociatedArchetypes.Num() * 0.05f + Memory.AccessCount * 0.02f;
}

bool FHexademic6CognitiveLattice::ShouldPromoteMemory(const FHexademicMemoryNode& Memory) const
{
    // Uses the inline method defined in FHexademicMemoryNode for promotion criteria.
    return Memory.ShouldPromoteToHigherOrder();
}

bool FHexademic6CognitiveLattice::ShouldDemoteMemory(const FHexademicMemoryNode& Memory) const
{
    // Uses the inline method defined in FHexademicMemoryNode for demotion criteria.
    return Memory.ShouldDecayToLowerOrder();
}
