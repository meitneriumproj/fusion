add_library(fusion STATIC)

target_sources(fusion
    PRIVATE

    # Core
    core/Instance.cpp
    core/Device.cpp
    core/Surface.cpp
    core/Swapchain.cpp
    frame/FrameContext.cpp
    frame/InFlightManager.cpp
    frame/FrameSystem.cpp
    memory/Allocator.cpp
    memory/MemoryPool.cpp
    resource/Buffer.cpp
    resource/Texture.cpp
    pipeline/GraphicsPipeline.cpp
    pipeline/ComputePipeline.cpp
    pipeline/PipelineBuilder.cpp
    graph/PassBuilder.cpp
    graph/PassContext.cpp
    graph/DependencyResolver.cpp
    graph/RenderGraph.cpp
    sync/Timeline.cpp
    sync/BarrierBuilder.cpp
    util/Logger.cpp
)

target_include_directories(fusion
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(fusion
    PUBLIC Vulkan::Vulkan
)

target_compile_features(fusion PUBLIC cxx_std_20)