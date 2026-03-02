#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;

class ComputePipeline {
public:
    ComputePipeline(Device& device,
                    VkPipeline pipeline,
                    VkPipelineLayout layout);
    ~ComputePipeline();

    ComputePipeline(const ComputePipeline&) = delete;
    ComputePipeline& operator=(const ComputePipeline&) = delete;

    ComputePipeline(ComputePipeline&& other) noexcept;
    ComputePipeline& operator=(ComputePipeline&& other) noexcept;

    VkPipeline vkPipeline() const noexcept { return m_pipeline; }
    VkPipelineLayout layout() const noexcept { return m_layout; }

private:
    Device& m_deviceRef;

    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
};

}