#include <fusion/frame/FrameContext.hpp>
#include <fusion/core/Device.hpp>

namespace fusion {

FrameContext::FrameContext(Device& device,
                           VkCommandPool pool,
                           VkCommandBuffer cmd,
                           VkFence fence,
                           VkSemaphore imageAvailable,
                           VkSemaphore renderFinished)
    : m_deviceRef(device),
      m_commandPool(pool),
      m_commandBuffer(cmd),
      m_fence(fence),
      m_imageAvailable(imageAvailable),
      m_renderFinished(renderFinished)
{
}

void FrameContext::reset() {
    vkResetCommandPool(
        m_deviceRef.vkDevice(),
        m_commandPool,
        0);
}

}