#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;

class GraphicsPipeline {
public:
    GraphicsPipeline(Device& device,
                     VkPipeline pipeline,
                     VkPipelineLayout layout);
    ~GraphicsPipeline();

    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

    GraphicsPipeline(GraphicsPipeline&& other) noexcept;
    GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept;

    VkPipeline vkPipeline() const noexcept { return m_pipeline; }
    VkPipelineLayout layout() const noexcept { return m_layout; }

private:
    Device& m_deviceRef;

    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
};

}