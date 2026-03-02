

#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;

class FrameContext {
public:
    FrameContext(Device& device,
                 VkCommandPool commandPool,
                 VkCommandBuffer commandBuffer,
                 VkFence fence,
                 VkSemaphore imageAvailable,
                 VkSemaphore renderFinished);

    VkCommandBuffer commandBuffer() const noexcept { return m_commandBuffer; }
    VkFence fence() const noexcept { return m_fence; }
    VkSemaphore imageAvailable() const noexcept { return m_imageAvailable; }
    VkSemaphore renderFinished() const noexcept { return m_renderFinished; }

    void reset();

private:
    Device& m_deviceRef;

    VkCommandPool m_commandPool = VK_NULL_HANDLE;
    VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;

    VkFence m_fence = VK_NULL_HANDLE;
    VkSemaphore m_imageAvailable = VK_NULL_HANDLE;
    VkSemaphore m_renderFinished = VK_NULL_HANDLE;
};

}