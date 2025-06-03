// Hexademic6ServiceLocator.cpp
// Modified to register concrete service implementations.

#include "HexademicSixLattice.h"
#include "CoreMinimal.h" // For UE_LOG, MakeShared
#include "Templates/SharedPointer.h" // For TSharedPtr

// Forward declarations for the concrete service implementations.
// In a production environment, these would typically be included from their own .h files.
class FHexademic6CognitiveLattice; // Already implemented, but forward declare for clarity if its .h isn't included directly.
class FHexademic6ResonanceService; // Defined in Hexademic6ResonanceService.cpp
class FHexademic6MythicService;    // Defined in Hexademic6MythicService.cpp

// Define a log category for Hexademic Lattice operations (if not already defined elsewhere)
// DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);

// Static instances of concrete services (these are private to FHexademic6ServiceLocator,
// but for static member initialization, they are often placed in the global scope of the .cpp)
// However, the FHexademic6ServiceLocator uses a static Get() method to return its instance,
// so these would be members of that static instance, not global variables.
// The previous implementation correctly placed them as members of the FHexademic6ServiceLocator class.

void FHexademic6ServiceLocator::Initialize()
{
    // Register concrete implementations of the services
    // Assuming FHexademic6CognitiveLattice is defined and accessible
    Get().CognitiveLattice = MakeShared<FHexademic6CognitiveLattice>(); 
    
    // Now registering the actual concrete services defined in their respective .cpp files
    // You must ensure that FHexademic6ResonanceService and FHexademic6MythicService are
    // properly linked and accessible at compile time.
    Get().Resonance = MakeShared<FHexademic6ResonanceService>(); 
    Get().Mythic = MakeShared<FHexademic6MythicService>();     

    UE_LOG(LogHexademicLattice, Log, TEXT("FHexademic6ServiceLocator initialized. All services registered: %s"), AreAllServicesRegistered() ? TEXT("True") : TEXT("False"));
}

void FHexademic6ServiceLocator::Shutdown()
{
    Get().CognitiveLattice.Reset();
    Get().Resonance.Reset();
    Get().Mythic.Reset();
    UE_LOG(LogHexademicLattice, Log, TEXT("FHexademic6ServiceLocator shutdown. All services unregistered."));
}

