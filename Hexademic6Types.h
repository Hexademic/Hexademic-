// Hexademic6Types.h
// Defines common structs and enums for the Hexademic6Lattice module.

#pragma once

#include "CoreMinimal.h" // For FVector, FLinearColor, etc.

// =============================================================================
// HEXADEMIC⁶ CORE TYPES
// =============================================================================

// Defines a 6-dimensional vector for use in the Hexademic lattice space.
// Corresponds to the six dimensions: X, Y, Z, W, U (Temporal), V (Mythic).
USTRUCT(BlueprintType)
struct HEXADEMIC6LATTICE_API FVector6
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float X;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Y;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Z;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float W;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float U; // Fifth dimension: Temporal resonance
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float V; // Sixth dimension: Mythic depth

    // Default constructor
    FVector6()
        : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f), U(0.0f), V(0.0f) {}

    // Constructor with all components
    FVector6(float InX, float InY, float InZ, float InW, float InU, float InV)
        : X(InX), Y(InY), Z(InZ), W(InW), U(InU), V(InV) {}

    // Operator overloads for common vector operations (simplified for example)
    FVector6 operator+(const FVector6& Other) const
    {
        return FVector6(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W, U + Other.U, V + Other.V);
    }

    FVector6 operator-(const FVector6& Other) const
    {
        return FVector6(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W, U - Other.U, V - Other.V);
    }

    FVector6 operator*(float Scalar) const
    {
        return FVector6(X * Scalar, Y * Scalar, Z * Scalar, W * Scalar, U * Scalar, V * Scalar);
    }

    float SizeSquared() const
    {
        return X*X + Y*Y + Z*Z + W*W + U*U + V*V;
    }

    float Size() const
    {
        return FMath::Sqrt(SizeSquared());
    }

    FVector6 GetSafeNormal(float Tolerance = KINDA_SMALL_NUMBER) const
    {
        float S = Size();
        if (S > Tolerance)
        {
            return *this / S;
        }
        return FVector6();
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("X=%f Y=%f Z=%f W=%f U=%f V=%f"), X, Y, Z, W, U, V);
    }
};

// You can add more shared types or utility functions here as needed.
