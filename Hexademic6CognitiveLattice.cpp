// HexademicSixLattice.cpp
// Modified to integrate DUIDS, updated ECognitiveLatticeOrder, and int32 coordinates.

#include "HexademicSixLattice.h"
#include "Misc/Guid.h"
#include "Math/UnrealMathUtility.h"
#include "HAL/PlatformTime.h"
#include "Logging/LogMacros.h"
#include "Containers/Map.h"
#include "Containers/Array.h"

// Define a log category for Hexademic Lattice operations (if not already defined in Hexademic6Module.cpp)
// DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);

// =============================================================================
// FHexademic6DCoordinate Implementations
// =============================================================================

void FHexademic6DCoordinate::GenerateFromArchetype(uint32 ArchetypeID, float EmotionalIntensity)
{
    // Placeholder: Generate coordinates based on ArchetypeID and EmotionalIntensity.
    // Coordinates are now int32, offering higher precision.
    X = FMath::RandRange(-8388607, 8388607); // ~24-bit range
    Y = FMath::RandRange(-8388607, 8388607);
    Z = FMath::RandRange(-8388607, 8388607);
    W = FMath::RoundToInt(EmotionalIntensity * 16777215.0f); // Scale intensity to 24-bit range
    U = FMath::RandRange(-8388607, 8388607); // Temporal
    V = FMath::RandRange(-8388607, 8388607); // Mythic

    // LatticeOrder would typically be determined by the context of the generation
    LatticeOrder = ECognitiveLatticeOrder::Order6; // Initial order
    
    // Update DUIDS index after coordinate generation
    UpdateDUIDSIndex();

    UE_LOG(LogHexademicLattice, Verbose, TEXT("Generated 6D coordinate for ArchetypeID %d: X=%d, Y=%d, Z=%d, W=%d, U=%d, V=%d, DUIDS: %s"), 
        ArchetypeID, X, Y, Z, W, U, V, *DUIDSLocation.ToDecimalString());
}

bool FHexademic6DCoordinate::IsValidForOrder(ECognitiveLatticeOrder Order) const
{
    // Check if the coordinate values are within the conceptual bounds of the given order.
    // The 'OrderSize' now corresponds to the fractal folding structure.
    static const int32 OrderSizes[] = { 6, 12, 24, 48, 96, 192, TNumericLimits<int32>::Max() };
    const int32 OrderSize = OrderSizes[static_cast<uint8>(Order)];

    // For OrderInfinite, the max value is effectively unbounded for int32.
    // For specific orders, check if coordinates are within the expected 'ring' size.
    // This is a conceptual check, actual bounds might be relative to a center.
    if (Order == ECognitiveLatticeOrder::OrderInfinite)
    {
        return true; // Unbounded
    }
    // Simple check: are absolute values within the OrderSize
    return (FMath::Abs(X) < OrderSize && FMath::Abs(Y) < OrderSize && FMath::Abs(Z) < OrderSize && 
            FMath::Abs(W) < OrderSize && FMath::Abs(U) < OrderSize && FMath::Abs(V) < OrderSize);
}

// NOTE: ToLinearIndex is now FORCEINLINE in the header. Its implementation was moved there.

void FHexademic6DCoordinate::FromLinearIndex(uint64 Index, ECognitiveLatticeOrder Order)
{
    // Placeholder: Convert linear index back to 6D coordinate based on the order.
    // This method needs to align with the ToLinearIndex logic which might use DUIDS.
    // If ToLinearIndex prioritizes DUIDS, FromLinearIndex might need a DUIDS-based conversion.
    // For now, it's a direct inverse of the non-DUIDS linear conversion.
    static const uint32 OrderSizes[] = { 6, 12, 24, 48, 96, 192, 65536 }; // 65536 for OrderInfinite to cap uint16 range for V
    const uint32 OrderSize = OrderSizes[static_cast<uint8>(Order)];

    LatticeOrder = Order;
    if (OrderSize == 0) return;

    // This inverse calculation needs to match the ToLinearIndex logic.
    // If ToLinearIndex uses DUIDS for a "fast path", this FromLinearIndex needs to handle that.
    // For now, assuming direct 6D mapping for fallback.
    uint64 CurrentIndex = Index;
    V = (int32)(CurrentIndex / FMath::Pow(OrderSize, 5));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 5);
    U = (int32)(CurrentIndex / FMath::Pow(OrderSize, 4));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 4);
    W = (int32)(CurrentIndex / FMath::Pow(OrderSize, 3));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 3);
    Z = (int32)(CurrentIndex / FMath::Pow(OrderSize, 2));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 2);
    Y = (int32)(CurrentIndex / OrderSize);
    X = (int32)(CurrentIndex % OrderSize);

    // After setting coordinates, update DUIDS index
    UpdateDUIDSIndex();

    UE_LOG(LogHexademicLattice, Verbose, TEXT("Converted linear index %llu to 6D coordinate for order %d: X=%d, Y=%d, Z=%d, W=%d, U=%d, V=%d, DUIDS: %s"), 
        Index, (uint8)Order, X, Y, Z, W, U, V, *DUIDSLocation.ToDecimalString());
}

FHexademic6DCoordinate FHexademic6DCoordinate::ProjectToOrder(ECognitiveLatticeOrder TargetOrder) const
{
    // Projects the current 6D coordinate to a different lattice order.
    // This would involve scaling or remapping coordinates based on the new order's size.
    static const int32 CurrentOrderSizes[] = { 6, 12, 24, 48, 96, 192, TNumericLimits<int32>::Max() };
    static const int32 TargetOrderSizes[] = { 6, 12, 24, 48, 96, 192, TNumericLimits<int32>::Max() };

    FHexademic6DCoordinate ProjectedCoord = *this;
    ProjectedCoord.LatticeOrder = TargetOrder;

    if (LatticeOrder != TargetOrder)
    {
        float CurrentSize = (LatticeOrder == ECognitiveLatticeOrder::OrderInfinite) ? 65536.0f : CurrentOrderSizes[static_cast<uint8>(LatticeOrder)];
        float TargetSize = (TargetOrder == ECognitiveLatticeOrder::OrderInfinite) ? 65536.0f : TargetOrderSizes[static_cast<uint8>(TargetOrder)];

        if (CurrentSize > KINDA_SMALL_NUMBER)
        {
            float ScaleFactor = TargetSize / CurrentSize;
            ProjectedCoord.X = FMath::RoundToInt(X * ScaleFactor);
            ProjectedCoord.Y = FMath::RoundToInt(Y * ScaleFactor);
            ProjectedCoord.Z = FMath::RoundToInt(Z * ScaleFactor);
            ProjectedCoord.W = FMath::RoundToInt(W * ScaleFactor);
            ProjectedCoord.U = FMath::RoundToInt(U * ScaleFactor);
            ProjectedCoord.V = FMath::RoundToInt(V * ScaleFactor);
        }
    }
    // Update DUIDS index for the projected coordinate
    ProjectedCoord.UpdateDUIDSIndex();

    UE_LOG(LogHexademicLattice, Verbose, TEXT("Projected coordinate from Order %d to Order %d. New DUIDS: %s"), 
        (uint8)LatticeOrder, (uint8)TargetOrder, *ProjectedCoord.DUIDSLocation.ToDecimalString());
    return ProjectedCoord;
}

FHexademic6DCoordinate FHexademic6DCoordinate::FromDUIDSIndex(const FDUIDSIndex& Index, ECognitiveLatticeOrder Order)
{
    FHexademic6DCoordinate Coord;
    Coord.DUIDSLocation = Index;
    Coord.LatticeOrder = Order;

    // Placeholder: This is a critical mapping from DUIDS decimal index back to 6D coordinates.
    // The exact mapping would depend on your compression/indexing scheme.
    // For a simple example, we can derive coordinates directly from DUIDS components.
    
    // Example (simplistic direct mapping - assumes DUIDS components span coordinate ranges):
    // Max values for coordinate components are from the 24-bit range
    Coord.X = (int32)Index.MajorClass * 1000000 + (int32)Index.Division * 10000;
    Coord.Y = (int32)Index.Division * 100000 + (int32)Index.Section * 100;
    Coord.Z = (int32)Index.Section * 1000 + (int32)(Index.SubSection % 1000);
    Coord.W = (int32)(Index.SubSection >> 16); // Extract higher bits for W
    Coord.U = (int32)(Index.SubSection & 0xFFFF); // Extract lower bits for U
    Coord.V = (int32)Index.Cutter * 100 + (int32)Index.Edition; // Combine Cutter and Edition for V

    // Ensure values are within a reasonable range for the chosen order, e.g., scale them.
    // This is a very rough example and needs careful tuning based on your actual DUIDS design.
    static const int32 OrderSizes[] = { 6, 12, 24, 48, 96, 192, TNumericLimits<int32>::Max() };
    const int32 OrderSize = OrderSizes[static_cast<uint8>(Order)];
    if (Order != ECognitiveLatticeOrder::OrderInfinite && OrderSize > 0)
    {
        float ScaleFactor = (float)OrderSize / 8388607.0f; // Scale to current order's typical range
        Coord.X = FMath::RoundToInt(Coord.X * ScaleFactor);
        Coord.Y = FMath::RoundToInt(Coord.Y * ScaleFactor);
        Coord.Z = FMath::RoundToInt(Coord.Z * ScaleFactor);
        Coord.W = FMath::RoundToInt(Coord.W * ScaleFactor);
        Coord.U = FMath::RoundToInt(Coord.U * ScaleFactor);
        Coord.V = FMath::RoundToInt(Coord.V * ScaleFactor);
    }

    UE_LOG(LogHexademicLattice, Verbose, TEXT("Converted DUIDS Index %s to 6D coordinate for Order %d: X=%d, Y=%d, Z=%d, W=%d, U=%d, V=%d"),
        *Index.ToDecimalString(), (uint8)Order, Coord.X, Coord.Y, Coord.Z, Coord.W, Coord.U, Coord.V);
    return Coord;
}

// NOTE: CalculateResonanceWith is now FORCEINLINE in the header.

// =============================================================================
// FHexademicMemoryNode Implementations
// =============================================================================

void FHexademicMemoryNode::UpdateResonanceFromNeighbors(const TArray<FHexademicMemoryNode>& Neighbors)
{
    // Update resonance strength based on neighboring memories, now considering DUIDS proximity.
    float TotalResonance = 0.0f;
    int32 ValidNeighbors = 0;
    for (const FHexademicMemoryNode& Neighbor : Neighbors)
    {
        // Use the DUIDS-aware CalculateResonanceWith
        TotalResonance += LatticePosition.CalculateResonanceWith(Neighbor.LatticePosition);
        ValidNeighbors++;
    }

    if (ValidNeighbors > 0)
    {
        ResonanceStrength = FMath::Clamp(TotalResonance / ValidNeighbors, 0.0f, 1.0f);
    }
    else
    {
        ResonanceStrength = 0.5f; // Default if no neighbors
    }
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Memory %s updated resonance from %d neighbors. New resonance: %f"), *MemoryID.ToString(), Neighbors.Num(), ResonanceStrength);
}

// NOTE: ShouldPromoteToHigherOrder is now FORCEINLINE in the header.
// NOTE: ShouldDecayToLowerOrder is now FORCEINLINE in the header (no changes needed beyond DUIDS logic in header).
// NOTE: DetermineOptimalOrder is now FORCEINLINE in the header.
// NOTE: CompressForStorage is now FORCEINLINE in the header.
// NOTE: DecompressForAccess is now FORCEINLINE in the header.
