// HexademicSixLattice.cpp
// This file implements utility methods for FHexademic6DCoordinate and FHexademicMemoryNode.

#include "HexademicSixLattice.h"
#include "Misc/Guid.h"
#include "Math/UnrealMathUtility.h"
#include "HAL/PlatformTime.h"
#include "Logging/LogMacros.h"
#include "Containers/Map.h"

// Define a log category for Hexademic Lattice operations
DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);

// =============================================================================
// FHexademic6DCoordinate Implementations
// =============================================================================

void FHexademic6DCoordinate::GenerateFromArchetype(uint32 ArchetypeID, float EmotionalIntensity)
{
    // Placeholder: Generate coordinates based on ArchetypeID and EmotionalIntensity.
    // In a full implementation, this would involve a complex mapping function, potentially
    // using a data asset for archetypal definitions. For demonstration, values are
    // derived simply from ArchetypeID and scaled EmotionalIntensity.
    X = (ArchetypeID * 7) % 256; 
    Y = (ArchetypeID * 13) % 256;
    Z = (ArchetypeID * 17) % 256;
    W = (ArchetypeID * 19) % 256;
    U = (uint16)(EmotionalIntensity * 255.0f); // Temporal resonance scaled to 0-255
    V = (ArchetypeID * 23) % 256; // Mythic depth based on archetype, scaled to 0-255

    // Initial lattice order for newly generated coordinates
    LatticeOrder = ECognitiveLatticeOrder::Order12;
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Generated 6D coordinate for ArchetypeID %d: X=%d, Y=%d, Z=%d, W=%d, U=%d, V=%d"), 
        ArchetypeID, X, Y, Z, W, U, V);
}

bool FHexademic6DCoordinate::IsValidForOrder(ECognitiveLatticeOrder Order) const
{
    // Checks if the coordinate values are within the conceptual bounds of the given lattice order.
    // The 'OrderSize' represents the maximum extent of each dimension for that specific order.
    uint32 OrderSize = 0;
    switch (Order)
    {
        case ECognitiveLatticeOrder::Order12: OrderSize = 12; break;
        case ECognitiveLatticeOrder::Order18: OrderSize = 18; break;
        case ECognitiveLatticeOrder::Order36: OrderSize = 36; break;
        case ECognitiveLatticeOrder::Order72: OrderSize = 72; break;
        case ECognitiveLatticeOrder::Order144: OrderSize = 144; break;
        case ECognitiveLatticeOrder::OrderInfinite: OrderSize = 65536; break; // uint16 max for unbounded
        default: return false;
    }

    return (X < OrderSize && Y < OrderSize && Z < OrderSize && 
            W < OrderSize && U < OrderSize && V < OrderSize);
}

void FHexademic6DCoordinate::FromLinearIndex(uint64 Index, ECognitiveLatticeOrder Order)
{
    // Converts a linear (1D) index back into a 6D coordinate for a given lattice order.
    // This assumes a continuous, compact linear indexing scheme.
    uint32 OrderSize = 0;
    switch (Order)
    {
        case ECognitiveLatticeOrder::Order12: OrderSize = 12; break;
        case ECognitiveLatticeOrder::Order18: OrderSize = 18; break;
        case ECognitiveLatticeOrder::Order36: OrderSize = 36; break;
        case ECognitiveLatticeOrder::Order72: OrderSize = 72; break;
        case ECognitiveLatticeOrder::Order144: OrderSize = 144; break;
        case ECognitiveLatticeOrder::OrderInfinite: OrderSize = 65536; break;
        default: return;
    }

    LatticeOrder = Order;
    if (OrderSize == 0) return; // Prevent division by zero

    // Calculate each dimension from the linear index
    uint64 CurrentIndex = Index;
    V = (uint16)(CurrentIndex / FMath::Pow(OrderSize, 5));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 5);
    U = (uint16)(CurrentIndex / FMath::Pow(OrderSize, 4));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 4);
    W = (uint16)(CurrentIndex / FMath::Pow(OrderSize, 3));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 3);
    Z = (uint16)(CurrentIndex / FMath::Pow(OrderSize, 2));
    CurrentIndex %= (uint64)FMath::Pow(OrderSize, 2);
    Y = (uint16)(CurrentIndex / OrderSize);
    X = (uint16)(CurrentIndex % OrderSize);

    UE_LOG(LogHexademicLattice, Verbose, TEXT("Converted linear index %llu to 6D coordinate for order %d: X=%d, Y=%d, Z=%d, W=%d, U=%d, V=%d"), 
        Index, (uint8)Order, X, Y, Z, W, U, V);
}

// =============================================================================
// FHexademicMemoryNode Implementations
// =============================================================================

void FHexademicMemoryNode::UpdateResonanceFromNeighbors(const TArray<FHexademicMemoryNode>& Neighbors)
{
    // Placeholder: Updates the memory's resonance strength based on its neighboring memories.
    // A more sophisticated implementation would consider type of connection, temporal proximity, etc.
    float TotalResonance = 0.0f;
    int32 ValidNeighbors = 0;
    for (const FHexademicMemoryNode& Neighbor : Neighbors)
    {
        // Simple example: add resonance based on spatial proximity
        TotalResonance += LatticePosition.CalculateResonanceWith(Neighbor.LatticePosition);
        ValidNeighbors++;
    }

    if (ValidNeighbors > 0)
    {
        ResonanceStrength = FMath::Clamp(TotalResonance / ValidNeighbors, 0.0f, 1.0f);
    }
    else
    {
        ResonanceStrength = 0.5f; // Default if no neighbors found
    }
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Memory %s updated resonance from %d neighbors. New resonance: %f"), *MemoryID.ToString(), Neighbors.Num(), ResonanceStrength);
}

ECognitiveLatticeOrder FHexademicMemoryNode::DetermineOptimalOrder() const
{
    // Determines the most suitable cognitive lattice order for this memory based on its current
    // state (access, resonance, decay, mythic depth).
    if (ShouldPromoteToHigherOrder())
    {
        switch (LatticePosition.LatticeOrder)
        {
            case ECognitiveLatticeOrder::Order12: return ECognitiveLatticeOrder::Order18;
            case ECognitiveLatticeOrder::Order18: return ECognitiveLatticeOrder::Order36;
            case ECognitiveLatticeOrder::Order36: return ECognitiveLatticeOrder::Order72;
            case ECognitiveLatticeOrder::Order72: return ECognitiveLatticeOrder::Order144;
            case ECognitiveLatticeOrder::Order144: return ECognitiveLatticeOrder::OrderInfinite;
            default: return ECognitiveLatticeOrder::OrderInfinite; // Cannot promote further
        }
    }
    else if (ShouldDecayToLowerOrder())
    {
        switch (LatticePosition.LatticeOrder)
        {
            case ECognitiveLatticeOrder::OrderInfinite: return ECognitiveLatticeOrder::Order144;
            case ECognitiveLatticeOrder::Order144: return ECognitiveLatticeOrder::Order72;
            case ECognitiveLatticeOrder::Order72: return ECognitiveLatticeOrder::Order36;
            case ECognitiveLatticeOrder::Order36: return ECognitiveLatticeOrder::Order18;
            case ECognitiveLatticeOrder::Order18: return ECognitiveLatticeOrder::Order12;
            default: return ECognitiveLatticeOrder::Order12; // Cannot demote further
        }
    }
    return LatticePosition.LatticeOrder; // No change needed
}
