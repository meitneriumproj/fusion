#include <fusion/pipeline/GraphicsPipeline.hpp>
#include <fusion/core/Device.hpp>

namespace fusion {

GraphicsPipeline::GraphicsPipeline(Device& device,
                                   VkPipeline pipeline,
                                   VkPipelineLayout layout)
    : m_deviceRef(device),
      m_pipeline(pipeline),
      m_layout(layout)
{
}

GraphicsPipeline::~GraphicsPipeline() {
    if (m_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(
            m_deviceRef.vkDevice(),
            m_pipeline,
            nullptr);
    }

    if (m_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(
            m_deviceRef.vkDevice(),
            m_layout,
            nullptr);
    }
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) noexcept
    : m_deviceRef(other.m_deviceRef),
      m_pipeline(other.m_pipeline),
      m_layout(other.m_layout)
{
    other.m_pipeline = VK_NULL_HANDLE;
    other.m_layout = VK_NULL_HANDLE;
}

GraphicsPipeline& GraphicsPipeline::operator=(GraphicsPipeline&& other) noexcept {
    if (this != &other) {
        m_pipeline = other.m_pipeline;
        m_layout = other.m_layout;
        other.m_pipeline = VK_NULL_HANDLE;
        other.m_layout = VK_NULL_HANDLE;
    }
    return *this;
}

}