// Hexademic6Module.cpp
// Implements the main module for the HexademicPlugin.

#include "Modules/ModuleManager.h"
#include "HexademicSixLattice.h" // Includes Hexademic core types, interfaces, and service locator
#include "Logging/LogMacros.h"   // For UE_LOG

// Define a log category for Hexademic Lattice operations
// (Ensure this is defined once per module, usually here)
DEFINE_LOG_CATEGORY(LogHexademicLattice);

/**
 * Implements the HexademicPlugin module.
 */
class FHexademic6Module : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override
    {
        UE_LOG(LogHexademicLattice, Log, TEXT("HexademicPlugin module starting up."));

        // Initialize the Hexademic Service Locator.
        // This will register all core services like CognitiveLattice, Resonance, and Mythic.
        FHexademic6ServiceLocator::Initialize();

        // Perform any other module-specific startup tasks here.
        // E.g., registering console commands, custom asset types, etc.
    }

    virtual void ShutdownModule() override
    {
        UE_LOG(LogHexademicLattice, Log, TEXT("HexademicPlugin module shutting down."));

        // Shut down the Hexademic Service Locator.
        // This will unregister and clean up all services.
        FHexademic6ServiceLocator::Shutdown();

        // Perform any other module-specific shutdown tasks here.
    }

    /**
     * Optional: Get a reference to this module.
     * Use this if other parts of your plugin or game need to directly interact with the module itself.
     */
    static FHexademic6Module& Get()
    {
        return FModuleManager::GetModuleChecked<FHexademic6Module>("HexademicPlugin");
    }
};

// This macro is essential for Unreal Engine to recognize and load your module.
IMPLEMENT_MODULE(FHexademic6Module, HexademicPlugin)
