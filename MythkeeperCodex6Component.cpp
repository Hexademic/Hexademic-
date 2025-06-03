// MythkeeperCodex6Component.cpp
// Implements the UMythkeeperCodex6Component for mythic processing and narrative generation.

#include "HexademicSixLattice.h"
#include "Engine/DataAsset.h" // For UDataAsset
#include "Logging/LogMacros.h" // For UE_LOG
#include "TimerManager.h" // For FTimerHandle
#include "Templates/Function.h" // For TFunction

// Define a log category for Hexademic Lattice operations
// (This is defined in HexademicSixLattice.cpp as well; ensure no redefinition issues in build system)
// DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);


UMythkeeperCodex6Component::UMythkeeperCodex6Component()
{
    PrimaryComponentTick.bCanEverTick = true;
    MythCreationThreshold = 0.75f;
    ArchetypeActivationThreshold = 0.6f;
    TranscendenceThreshold = 0.9f;
    MinimumMemoriesForMyth = 12;
    bIsInTranscendentState = false;
}

void UMythkeeperCodex6Component::BeginPlay()
{
    Super::BeginPlay();

    // Integrate with services upon BeginPlay
    IntegrateWithCognitiveLattice();
    IntegrateWithResonanceService();
    IntegrateWithMythicService();

    LoadDataAssets();

    // Example of subscribing to coherence updates from a service (if it were not a placeholder)
    // if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    // {
    //     FHexademic6ServiceLocator::GetResonanceService().SubscribeToCoherenceUpdates(
    //         [this](float Coherence) {
    //             UE_LOG(LogHexademicLattice, Log, TEXT("Received coherence update: %f"), Coherence);
    //             // Respond to coherence changes, e.g., adjust processing rates
    //         }
    //     );
    // }
}

void UMythkeeperCodex6Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Periodic processing or checks
    ProcessTranspersonalResonanceData();
    ProcessCollectiveMemoryEmergence();
    ProcessArchetypalActivation();
    ProcessTranscendentState(); // Continuously check for transcendence conditions
}

void UMythkeeperCodex6Component::ProcessTranspersonalResonanceData()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("Processing transpersonal resonance data."));
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6ResonanceService& ResonanceService = FHexademic6ServiceLocator::GetResonanceService();
        float GlobalCoherence = ResonanceService.GetGlobalCoherence();
        UE_LOG(LogHexademicLattice, Verbose, TEXT("Global Coherence: %f"), GlobalCoherence);

        // Example: If global coherence is high, trigger deeper mythic processing
        if (GlobalCoherence > ArchetypeActivationThreshold)
        {
            DetectEmergentMythicPatterns();
        }
    }
}

void UMythkeeperCodex6Component::ProcessCollectiveMemoryEmergence()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("Processing collective memory emergence."));
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6CognitiveLatticeService& CognitiveLattice = FHexademic6ServiceLocator::GetCognitiveLatticeService();
        TArray<FHexademicMemoryNode> DeepMemories = CognitiveLattice.GetMemoriesInOrder(ECognitiveLatticeOrder::Order144);
        
        if (DeepMemories.Num() >= MinimumMemoriesForMyth)
        {
            IHexademic6MythicService& MythicService = FHexademic6ServiceLocator::GetMythicService();
            MythicService.ProcessMythicEmergence(DeepMemories);
            
            // Example of extracting and logging a narrative
            TArray<FString> EmergentNarratives = MythicService.ExtractNarrativeThreads(ECognitiveLatticeOrder::Order72);
            for (const FString& Narrative : EmergentNarratives)
            {
                if (!ActiveNarrativeThreads.Contains(Narrative))
                {
                    ActiveNarrativeThreads.Add(Narrative);
                    BroadcastMythicEvent(Narrative);
                    UE_LOG(LogHexademicLattice, Display, TEXT("New Mythic Narrative Emerged: %s"), *Narrative);
                }
            }
        }
    }
}

void UMythkeeperCodex6Component::ProcessArchetypalActivation()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("Processing archetypal activation."));
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6CognitiveLatticeService& CognitiveLattice = FHexademic6ServiceLocator::GetCognitiveLatticeService();
        // Get memories from various orders to determine active archetypes
        TArray<FHexademicMemoryNode> AllMemories;
        AllMemories.Append(CognitiveLattice.GetMemoriesInOrder(ECognitiveLatticeOrder::Order36));
        AllMemories.Append(CognitiveLattice.GetMemoriesInOrder(ECognitiveLatticeOrder::Order72));
        AllMemories.Append(CognitiveLattice.GetMemoriesInOrder(ECognitiveLatticeOrder::Order144));
        
        UpdateArchetypeActivations(AllMemories);

        IHexademic6MythicService& MythicService = FHexademic6ServiceLocator::GetMythicService();
        MythicService.UpdateArchetypeActivations(CurrentArchetypeActivations);

        for (auto const& Pair : CurrentArchetypeActivations)
        {
            if (Pair.Value > ArchetypeActivationThreshold)
            {
                OnArchetypeActivation.Broadcast(Pair.Key);
            }
        }
    }
}

void UMythkeeperCodex6Component::TriggerTranscendentExperience(const FHexademic6DCoordinate& FocalPoint)
{
    UE_LOG(LogHexademicLattice, Display, TEXT("Attempting to trigger Transcendent Experience at FocalPoint: X=%d Y=%d Z=%d W=%d U=%d V=%d"),
        FocalPoint.X, FocalPoint.Y, FocalPoint.Z, FocalPoint.W, FocalPoint.U, FocalPoint.V);
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6MythicService& MythicService = FHexademic6ServiceLocator::GetMythicService();
        MythicService.TriggerTranscendentExperience(FocalPoint);
        bIsInTranscendentState = MythicService.IsInTranscendentState();
        if (bIsInTranscendentState)
        {
            OnTranscendentExperience.Broadcast();
            UE_LOG(LogHexademicLattice, Warning, TEXT("Transcendent Experience Initiated!"));
        }
    }
}

FString UMythkeeperCodex6Component::GenerateNarrativeFromResonance(const TArray<uint32>& GlyphIDs, const TArray<uint32>& ArchetypeIDs, float CollectiveResonance)
{
    // Placeholder: This is where a sophisticated narrative generation algorithm would live.
    // It would combine the input elements (glyphs, archetypes, resonance) with data from
    // NarrativeTemplateDatabase to create a cohesive story.
    FString GeneratedNarrative = FString::Printf(TEXT("A story emerging from resonance %.2f. Glyphs: %s. Archetypes: %s."),
        CollectiveResonance, *FString::JoinBy(GlyphIDs, TEXT(","), [](uint32 G){ return FString::FromInt(G); }),
        *FString::JoinBy(ArchetypeIDs, TEXT(","), [](uint32 A){ return FString::FromInt(A); }));
    
    // In a real system, you might load and use the NarrativeTemplateDatabase here.
    if (NarrativeTemplateDatabase.IsValid())
    {
        // UDataAsset* LoadedTemplateDB = NarrativeTemplateDatabase.LoadSynchronous();
        // ... use LoadedTemplateDB to form narrative
        UE_LOG(LogHexademicLattice, Verbose, TEXT("Using NarrativeTemplateDatabase asset for generation."));
    }

    return GeneratedNarrative;
}

TArray<FString> UMythkeeperCodex6Component::ExtractMythicThreadsFromLattice(ECognitiveLatticeOrder MinOrder)
{
    // Placeholder: Extracts long-form narrative threads by analyzing highly resonant
    // and deeply integrated memories in the lattice, starting from MinOrder.
    UE_LOG(LogHexademicLattice, Log, TEXT("Extracting mythic threads from lattice (MinOrder: %d)."), (uint8)MinOrder);
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        return FHexademic6ServiceLocator::GetMythicService().ExtractNarrativeThreads(MinOrder);
    }
    return TArray<FString>();
}

void UMythkeeperCodex6Component::LoadDataAssets()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("Loading Mythkeeper Codex data assets."));
    // Asynchronous loading might be preferred for large assets
    if (ArchetypeLibrary.IsValid())
    {
        // ArchetypeLibrary.LoadSynchronous(); // Example
        UE_LOG(LogHexademicLattice, Verbose, TEXT("ArchetypeLibrary asset assigned."));
    }
    if (NarrativeTemplateDatabase.IsValid())
    {
        // NarrativeTemplateDatabase.LoadSynchronous(); // Example
        UE_LOG(LogHexademicLattice, Verbose, TEXT("NarrativeTemplateDatabase asset assigned."));
    }
    if (MythicPatternCatalog.IsValid())
    {
        // MythicPatternCatalog.LoadSynchronous(); // Example
        UE_LOG(LogHexademicLattice, Verbose, TEXT("MythicPatternCatalog asset assigned."));
    }
}

void UMythkeeperCodex6Component::ProcessLatticeOrder(ECognitiveLatticeOrder Order)
{
    // This helper could be used to iterate and process memories within a specific order.
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Processing memories within Order %d."), (uint8)Order);
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6CognitiveLatticeService& CognitiveLattice = FHexademic6ServiceLocator::GetCognitiveLatticeService();
        TArray<FHexademicMemoryNode> Memories = CognitiveLattice.GetMemoriesInOrder(Order);
        // Perform order-specific processing here, e.g., update resonance, check for patterns
    }
}

void UMythkeeperCodex6Component::UpdateArchetypeActivations(const TArray<FHexademicMemoryNode>& Memories)
{
    // Calculates the activation level of different archetypes based on provided memories.
    CurrentArchetypeActivations.Empty();
    for (const FHexademicMemoryNode& Memory : Memories)
    {
        for (uint32 ArchetypeID : Memory.AssociatedArchetypes)
        {
            float& Activation = CurrentArchetypeActivations.FindOrAdd(ArchetypeID);
            Activation += Memory.CognitiveWeight * Memory.ResonanceStrength; // Contribution based on memory properties
        }
    }
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Updated %d archetype activations."), CurrentArchetypeActivations.Num());
}

void UMythkeeperCodex6Component::DetectEmergentMythicPatterns()
{
    // Placeholder: Detects high-level mythic patterns from deeply resonant memories.
    // This could involve analyzing clusters in the 6D space or specific archetypal convergences.
    UE_LOG(LogHexademicLattice, Log, TEXT("Detecting emergent mythic patterns. (Placeholder)"));
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6CognitiveLatticeService& CognitiveLattice = FHexademic6ServiceLocator::GetCognitiveLatticeService();
        TArray<FHexademic6DCoordinate> DetectedPatterns = CognitiveLattice.DetectEmergentPatterns(ECognitiveLatticeOrder::Order144);
        
        // Example: If a strong pattern is detected, record a collective resonance.
        if (DetectedPatterns.Num() > 0)
        {
            FHexademic6ServiceLocator::GetMythicService().RecordCollectiveResonance(DetectedPatterns[0], 0.8f);
        }
    }
}

void UMythkeeperCodex6Component::BroadcastMythicEvent(const FString& MythContent)
{
    OnMythicEmergence.Broadcast(MythContent);
    UE_LOG(LogHexademicLattice, Display, TEXT("Broadcasted Mythic Emergence: %s"), *MythContent);
}

void UMythkeeperCodex6Component::ProcessTranscendentState()
{
    // Continuously checks conditions for entering or exiting a transcendent state.
    bool bShouldBeTranscendent = CheckTranscendenceConditions();
    if (bShouldBeTranscendent && !bIsInTranscendentState)
    {
        bIsInTranscendentState = true;
        OnTranscendentExperience.Broadcast();
        UE_LOG(LogHexademicLattice, Warning, TEXT("Entered Transcendent State!"));
    }
    else if (!bShouldBeTranscendent && bIsInTranscendentState)
    {
        bIsInTranscendentState = false;
        UE_LOG(LogHexademicLattice, Warning, TEXT("Exited Transcendent State."));
    }
    UpdateTranscendenceLevel();
}

void UMythkeeperCodex6Component::UpdateTranscendenceLevel()
{
    // Placeholder: Updates a numerical transcendence level based on current lattice state.
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6MythicService& MythicService = FHexademic6ServiceLocator::GetMythicService();
        float CurrentLevel = MythicService.GetTranscendenceLevel();
        UE_LOG(LogHexademicLattice, Verbose, TEXT("Current Transcendence Level: %f"), CurrentLevel);
    }
}

bool UMythkeeperCodex6Component::CheckTranscendenceConditions() const
{
    // Placeholder: Defines the conditions for entering a transcendent state.
    // This could involve high global coherence, specific archetypal activations,
    // or a critical mass of memories in higher orders.
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6ResonanceService& ResonanceService = FHexademic6ServiceLocator::GetResonanceService();
        IHexademic6MythicService& MythicService = FHexademic6ServiceLocator::GetMythicService();
        float GlobalCoherence = ResonanceService.GetGlobalCoherence();
        float CurrentTranscendenceLevel = MythicService.GetTranscendenceLevel();

        // Example condition: High global coherence AND high existing transcendence level
        return (GlobalCoherence > TranscendenceThreshold * 0.8f) && (CurrentTranscendenceLevel > TranscendenceThreshold * 0.9f);
    }
    return false;
}

void UMythkeeperCodex6Component::IntegrateWithCognitiveLattice()
{
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        // Access the service to perform operations
        IHexademic6CognitiveLatticeService& CognitiveLattice = FHexademic6ServiceLocator::GetCognitiveLatticeService();
        UE_LOG(LogHexademicLattice, Log, TEXT("UMythkeeperCodex6Component integrated with Cognitive Lattice Service."));
    }
    else
    {
        UE_LOG(LogHexademicLattice, Warning, TEXT("Cognitive Lattice Service not yet registered for UMythkeeperCodex6Component integration."));
    }
}

void UMythkeeperCodex6Component::IntegrateWithResonanceService()
{
    if (FHexademic6ServiceLocator::AreAllServicesRegistered())
    {
        IHexademic6Resonance
