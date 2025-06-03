// Hexademic6ComputeComponent.cpp
// Modified to integrate and dispatch compute shaders for lattice operations.

#include "HexademicSixLattice.h"
#include "Hexademic6Types.h" // For FVector6 and other shared types
#include "RenderCore/Public/RenderCore.h" // For FComputeBufferRHIRef
#include "Runtime/Engine/Classes/Engine/ComputeShader.h" // For UComputeShader
#include "Runtime/RHI/Public/RHI.h" // For RHICmdList, RHI types
#include "Runtime/RHI/Public/RHIGlobals.h" // For GRHIMaxDispatchSize
#include "RenderingThread.h" // For ENQUEUE_RENDER_COMMAND
#include "Logging/LogMacros.h" // For UE_LOG
#include "TimerManager.h" // For FTimerHandle
#include "ShaderCore.h" // For FGlobalShaderMap, FindShaderPermutation
#include "GlobalShader.h" // For FGlobalShader, TShaderMapRef
#include "Misc/ScopeLock.h" // For FScopeLock
#include "UObject/ConstructorHelpers.h" // For ConstructorHelpers::FObjectFinder

// Define a log category for Hexademic Lattice operations
// (Ensure this is defined once per module, e.g., in HexademicSixLattice.cpp or Hexademic6Module.cpp)
// DEFINE_LOG_CATEGORY_STATIC(LogHexademicLattice, Log, All);

// Forward declare the concrete service for interactions
class FHexademic6CognitiveLattice; 
// In a full setup, you'd include the Hexademic6CognitiveLattice.h header here.

UHexademic6ComputeComponent::UHexademic6ComputeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bEnableGPUAcceleration = true;
    ResonanceUpdateInterval = 0.1f;
    MythicProcessingInterval = 1.0f;

    // // Example: Loading ComputeShader assets from content
    // // Make sure you have these assets created in your Content Browser and set in blueprints
    // static ConstructorHelpers::FObjectFinder<UComputeShader> LatticeShaderAsset(TEXT("/Game/Hexademic/Shaders/CS_LatticeEvolution.CS_LatticeEvolution"));
    // if (LatticeShaderAsset.Succeeded())
    // {
    //     LatticeComputeShader = LatticeShaderAsset.Object;
    // }
    // static ConstructorHelpers::FObjectFinder<UComputeShader> ResonanceShaderAsset(TEXT("/Game/Hexademic/Shaders/CS_ResonanceField.CS_ResonanceField"));
    // if (ResonanceShaderAsset.Succeeded())
    // {
    //     ResonanceComputeShader = ResonanceShaderAsset.Object;
    // }
    // static ConstructorHelpers::FObjectFinder<UComputeShader> MythicShaderAsset(TEXT("/Game/Hexademic/Shaders/CS_MythicDetection.CS_MythicDetection"));
    // if (MythicShaderAsset.Succeeded())
    // {
    //     MythicComputeShader = MythicShaderAsset.Object;
    // }
}

void UHexademic6ComputeComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bEnableGPUAcceleration)
    {
        InitializeGPUResources();
        // Start timers for periodic GPU dispatches
        GetWorld()->GetTimerManager().SetTimer(ResonanceTimer, this, &UHexademic6ComputeComponent::DispatchResonanceFieldUpdate, ResonanceUpdateInterval, true);
        GetWorld()->GetTimerManager().SetTimer(MythicTimer, this, &UHexademic6ComputeComponent::DispatchMythicPatternDetection, MythicProcessingInterval, true);
    }
}

void UHexademic6ComputeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // If direct CPU-GPU sync is needed every tick, call SynchronizeWithCPULattice() here.
}

void UHexademic6ComputeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(ResonanceTimer);
    GetWorld()->GetTimerManager().ClearTimer(MythicTimer);

    ReleaseGPUResources();
}

void UHexademic6ComputeComponent::DispatchResonanceFieldUpdate()
{
    if (!bEnableGPUAcceleration || !ResonanceComputeShader) return;

    UE_LOG(LogHexademicLattice, Log, TEXT("Dispatching GPU Resonance Field Update."));

    // Prepare CPU data and upload to GPU buffers
    UpdateGPUBuffers(); 

    // Define thread group size (e.g., total nodes / 64 threads per group)
    FIntVector ThreadGroups(FMath::DivideAndRoundUp(1024, 64), 1, 1); // Example: 1024 resonance field elements

    // Dispatch the compute shader on the render thread
    DispatchComputeShader(ResonanceComputeShader, TEXT("MainCS"), ThreadGroups);
}

void UHexademic6ComputeComponent::DispatchLatticeEvolution()
{
    if (!bEnableGPUAcceleration || !LatticeComputeShader) return;

    UE_LOG(LogHexademicLattice, Log, TEXT("Dispatching GPU Lattice Evolution."));

    UpdateGPUBuffers(); // Ensure latest data is on GPU

    // Assuming you have a way to get the total number of memory nodes for dispatch size
    // Example: FHexademic6ServiceLocator::GetCognitiveLatticeService().GetLatticeComplexity() could give a hint
    uint32 TotalNodes = 256; // Placeholder, replace with actual count
    FIntVector ThreadGroups(FMath::DivideAndRoundUp(TotalNodes, 64), 1, 1);

    DispatchComputeShader(LatticeComputeShader, TEXT("MainCS"), ThreadGroups);
}

void UHexademic6ComputeComponent::DispatchMythicPatternDetection()
{
    if (!bEnableGPUAcceleration || !MythicComputeShader) return;

    UE_LOG(LogHexademicLattice, Log, TEXT("Dispatching GPU Mythic Pattern Detection."));

    UpdateGPUBuffers(); // Ensure latest data is on GPU

    uint32 TotalDeepMemories = 100; // Placeholder, replace with actual count
    FIntVector ThreadGroups(FMath::DivideAndRoundUp(TotalDeepMemories, 32), 1, 1);

    DispatchComputeShader(MythicComputeShader, TEXT("MythicPatternDetectionCS"), ThreadGroups);
}

void UHexademic6ComputeComponent::SynchronizeWithCPULattice()
{
    if (!bEnableGPUAcceleration) return;

    UE_LOG(LogHexademicLattice, Log, TEXT("Synchronizing GPU data with CPU Lattice. (Placeholder for async readback)"));
    
    // This involves reading data back from GPU buffers (e.g., OutMemoryNodes, ArchetypeActivationBuffer)
    // and updating the CPU-side FHexademic6CognitiveLattice and UMythkeeperCodex6Component states.
    // This is typically an asynchronous process using FAsyncReadbackRequest or similar RHI utilities.
    // Due to the asynchronous nature, direct data transfer here is not feasible in a synchronous call.
    // You'd initiate a readback request, and then process the data once it's ready on the CPU side.
}

void UHexademic6ComputeComponent::InitializeGPUResources()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("Initializing GPU resources."));

    // Ensure RHI is initialized before creating resources
    if (!GRHISupportsRHIThread || !GRHIIsInitialized)
    {
        UE_LOG(LogHexademicLattice, Error, TEXT("RHI not initialized. Cannot initialize GPU resources."));
        bEnableGPUAcceleration = false;
        return;
    }

    // Allocate resources on the render thread
    ENQUEUE_RENDER_COMMAND(HexademicInitializeGPUResourcesCommand)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            // Create buffer for Resonance Field (example size 1024 floats)
            FRHIResourceCreateInfo ResonanceCreateInfo(TEXT("ResonanceFieldBuffer"));
            ResonanceFieldBuffer = RHICreateStructuredBuffer(sizeof(float), 1024 * sizeof(float), BUF_UnorderedAccess | BUF_ShaderResource, ERHIAccess::SRVCompute, ResonanceCreateInfo);
            
            // Create buffers for each lattice order (simplified placeholder for FHexademicMemoryNode_GPU)
            // Assuming max nodes per order for buffer size (e.g., 256 for a small demo)
            // Note: FHexademicMemoryNode_GPU size should match HLSL struct size.
            uint32 MemoryNodeGPUSize = sizeof(float) * 10; // Example size, adjust to actual HLSL struct size
            OrderBuffers.Empty();
            for (uint8 i = 0; i <= (uint8)ECognitiveLatticeOrder::OrderInfinite; ++i) // Including OrderInfinite for completeness
            {
                ECognitiveLatticeOrder Order = (ECognitiveLatticeOrder)i;
                FString BufferName = FString::Printf(TEXT("OrderBuffer_%d"), (int)Order);
                FRHIResourceCreateInfo OrderBufferCreateInfo(*BufferName);
                // Adjust max_nodes_per_order to actual planned capacity
                int32 MaxNodesPerOrder = (Order == ECognitiveLatticeOrder::OrderInfinite) ? 1000 : 256; 
                OrderBuffers.Add(Order, RHICreateStructuredBuffer(MemoryNodeGPUSize, MaxNodesPerOrder * MemoryNodeGPUSize, BUF_UnorderedAccess | BUF_ShaderResource, ERHIAccess::SRVCompute, OrderBufferCreateInfo));
            }

            // Archetype Activation Buffer (example size 256 archetypes)
            FRHIResourceCreateInfo ArchetypeBufferCreateInfo(TEXT("ArchetypeActivationBuffer"));
            uint32 ArchetypeGPUSize = sizeof(uint32) + sizeof(float); // ArchetypeID + ActivationValue
            ArchetypeActivationBuffer = RHICreateStructuredBuffer(ArchetypeGPUSize, 256 * ArchetypeGPUSize, BUF_UnorderedAccess | BUF_ShaderResource, ERHIAccess::SRVCompute, ArchetypeBufferCreateInfo);

            UE_LOG(LogHexademicLattice, Log, TEXT("GPU resources initialized on render thread."));
        });
}

void UHexademic6ComputeComponent::UpdateGPUBuffers()
{
    // This is where CPU data would be copied to GPU buffers on the render thread.
    // This operation typically happens inside ENQUEUE_RENDER_COMMAND.
    UE_LOG(LogHexademicLattice, Verbose, TEXT("Updating GPU buffers with CPU data. (Placeholder for data transfer)"));

    // Example of how you would queue a buffer update for a single buffer:
    // TArray<FHexademicMemoryNode_GPU> CPUTempData; // Populate this with actual data from CPU lattice
    // ENQUEUE_RENDER_COMMAND(UpdateMemoryBuffer)(
    //     [this, CPUTempData](FRHICommandListImmediate& RHICmdList)
    //     {
    //         if (OrderBuffers.Contains(ECognitiveLatticeOrder::Order12) && OrderBuffers[ECognitiveLatticeOrder::Order12].IsValid())
    //         {
    //             RHICmdList.UpdateBuffer(OrderBuffers[ECognitiveLatticeOrder::Order12]->Get, 0, CPUTempData.Num() * sizeof(FHexademicMemoryNode_GPU), CPUTempData.GetData());
    //         }
    //     }
    // );
}

void UHexademic6ComputeComponent::ReleaseGPUResources()
{
    UE_LOG(LogHexademicLattice, Log, TEXT("Releasing GPU resources."));

    ENQUEUE_RENDER_COMMAND(HexademicReleaseResourcesCommand)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            if (ResonanceFieldBuffer.IsValid())
            {
                ResonanceFieldBuffer.SafeRelease();
            }
            if (ArchetypeActivationBuffer.IsValid())
            {
                ArchetypeActivationBuffer.SafeRelease();
            }
            for (auto& Pair : OrderBuffers)
            {
                if (Pair.Value.IsValid())
                {
                    Pair.Value.SafeRelease();
                }
            }
            OrderBuffers.Empty();
            UE_LOG(LogHexademicLattice, Log, TEXT("GPU resources released on render thread."));
        });
}

void UHexademic6ComputeComponent::DispatchComputeShader(UComputeShader* Shader, const FString& KernelName, const FIntVector& ThreadGroups)
{
    if (!Shader || !Shader->GetResource())
    {
        UE_LOG(LogHexademicLattice, Error, TEXT("Attempted to dispatch null or invalid compute shader."));
        return;
    }

    UE_LOG(LogHexademicLattice, Log, TEXT("Dispatching compute shader '%s' with kernel '%s' and thread groups X:%d Y:%d Z:%d."),
        *Shader->GetName(), *KernelName, ThreadGroups.X, ThreadGroups.Y, ThreadGroups.Z);

    // This is the core RHI dispatch logic. It must run on the render thread.
    ENQUEUE_RENDER_COMMAND(HexademicDispatchComputeShaderCommand)(
        [this, Shader, KernelName, ThreadGroups](FRHICommandListImmediate& RHICmdList)
        {
            FGlobalShaderMap* GlobalShaderMap = Get={.Google Search_result=[{"snippet": "FGlobalShaderMap::Get ( FShaderMapID ) - Unreal Engine 4.27\nhttps://docs.unrealengine.com/4.27/en-US/API/Runtime/ShaderCore/FGlobalShaderMap/Get/\nFGlobalShaderMap is essentially a singleton, so you can access it via GetGlobalShaderMap()", "title": "FGlobalShaderMap::Get ( FShaderMapID ) - Unreal Engine 4.27", "link": "https://docs.unrealengine.com/4.27/en-US/API/Runtime/ShaderCore/FGlobalShaderMap/Get/"}]}
GetGlobalShaderMap(GMaxRHIShaderPlatform);
            TShaderMapRef<FComputeShader> ComputeShaderRef(GlobalShaderMap, Shader->GetResource()->GetShaderId());

            if (ComputeShaderRef.IsValid())
            {
                // Set the compute shader
                RHICmdList.SetComputeShader(ComputeShaderRef.GetComputeShader());

                // --- Binding Resources and Parameters ---
                // This section is highly dependent on your actual shader.
                // You would bind your SRVs (InMemoryNodes, DeepMemoriesBuffer) and UAVs (OutMemoryNodes, ResonanceField, ArchetypeActivationBuffer).
                // And set constant buffer parameters (PerFrameParameters, MythicParameters).

                // Example: Binding an input structured buffer (SRV)
                // ComputeShaderRef->SetSRV(RHICmdList, ComputeShaderRef->GetSRVParameter("InMemoryNodes"), OrderBuffers[ECognitiveLatticeOrder::Order12]->ShaderResourceViewRHI);

                // Example: Binding an output structured buffer (UAV)
                // ComputeShaderRef->SetUAV(RHICmdList, ComputeShaderRef->GetUAVParameter("OutMemoryNodes"), OrderBuffers[ECognitiveLatticeOrder::Order12]->UnorderedAccessViewRHI);

                // Example: Setting a constant buffer (assuming FMyShaderParameters is defined in your shader's C++ wrapper)
                // FMyShaderParameters Parameters; // Populate with CPU data
                // ComputeShaderRef->SetParameters(RHICmdList, Parameters); 

                // Dispatch the compute shader
                RHICmdList.DispatchComputeShader(ThreadGroups.X, ThreadGroups.Y, ThreadGroups.Z);

                // Unbind UAVs to ensure data is flushed and available for other passes/readback
                // This is crucial if another shader pass or CPU readback will access these resources.
                // RHICmdList.UnsetUAVs(ComputeShaderRef->GetUAVParameter("OutMemoryNodes"), 1);
                // RHICmdList.UnsetUAVs(ComputeShaderRef->GetUAVParameter("ResonanceField"), 1);
            }
            else
            {
                UE_LOG(LogHexademicLattice, Error, TEXT("Failed to get valid TShaderMapRef for compute shader %s."), *Shader->GetName());
            }
        });
}

void UHexademic6ComputeComponent::SetComputeShaderParameters(UComputeShader* Shader, const TMap<FString, float>& Parameters)
{
    // This function is for conceptual parameter setting.
    // In actual RHI dispatch, parameters are set directly on the render thread
    // using FShaderParameter objects or by setting constant buffers.
    UE_LOG(LogHexademicLattice, Log, TEXT("Setting parameters for compute shader '%s'. (Conceptual)"), *Shader->GetName());
}
