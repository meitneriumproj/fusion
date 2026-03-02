#pragma once

//
// Fusion API v1.00
//
// Minimal public API surface.
// Advanced systems available via explicit includes.
//

// Core
#include <fusion/core/Instance.hpp>
#include <fusion/core/Device.hpp>
#include <fusion/core/Surface.hpp>

// Frame system
#include <fusion/frame/FrameSystem.hpp>
#include <fusion/frame/FrameContext.hpp>

// Render graph
#include <fusion/graph/RenderGraph.hpp>
#include <fusion/graph/ResourceHandle.hpp>

// Resource descriptions (safe public exposure)
#include <fusion/resource/ResourceDescription.hpp>

// Pipelines (safe high-level exposure)
#include <fusion/pipeline/GraphicsPipeline.hpp>
#include <fusion/pipeline/ComputePipeline.hpp>
#include <fusion/pipeline/PipelineBuilder.hpp>

// Utilities
#include <fusion/util/Logger.hpp>

// FOR DEVS:
// Internal modules NOT included:
//
// memory/
// sync/
// dependency resolver
// pass builder
// pass context
// debug tools
//
// These are available via explicit include only.
//