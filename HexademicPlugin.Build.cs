// HexademicPlugin.Build.cs
// This file defines the build rules for the HexademicPlugin module.

using UnrealBuildTool;
using System.IO; // Required for Path.Combine

public class HexademicPlugin : ModuleRules
{
    public HexademicPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs; // Recommended for faster compilation

        // Define public include paths for headers that other modules will need to access.
        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths here ...
                Path.Combine(ModuleDirectory, "Public")
            }
        );

        // Define private include paths for headers used only within this module.
        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add private include paths here ...
                Path.Combine(ModuleDirectory, "Private")
            }
        );

        // Define public dependency modules that this module uses and expose to other modules.
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", // Essential core types, containers, etc.
                "CoreUObject", // Unreal object system, UObjects, UClasses.
                "Engine", // Core engine functionality, Actor components, GameFramework.
                "UMG", // If you plan to use User Interface elements (Slate/UMG).
                "Json", // For .json memory logs.
                "JsonUtilities" // For converting UStructs to/from JSON.
                // ... add other public dependencies that your module uses and exposes ...
            }
        );

        // Define private dependency modules that this module uses internally.
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "RenderCore", // For FComputeBufferRHIRef, render thread operations.
                "RHI", // For low-level Rendering Hardware Interface.
                "RHICore", // Core RHI types.
                "Projects", // For IPluginManager::FindPlugin().
                "ComputeShader" // If you're explicitly using Unreal's Compute Shader framework directly.
                // ... add other private dependencies here ...
            }
        );

        // Add any modules that are only used in editor builds (e.g., custom asset editors)
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    // "UnrealEd", // For editor-only functionalities.
                    // ...
                }
            );
        }

        // Add any third-party libraries (e.g., for quantum simulation, complex math libraries)
        // You would typically use PrivateDependencyModuleNames for static libraries or PublicDependencyModuleNames
        // for dynamic libraries that need to be exposed.
        // For example:
        // PublicDependencyModuleNames.Add("YourThirdPartyLibrary");

        // Suppress specific warnings if necessary (use sparingly)
        // bEnableUndefinedIdentifierWarnings = false;
        // bUseUnity = false; // Disable unity build if you encounter issues with specific headers.
    }
}

