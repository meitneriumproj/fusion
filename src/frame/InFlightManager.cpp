#include <fusion/frame/InFlightManager.hpp>
#include <fusion/frame/FrameContext.hpp>
#include <fusion/core/Device.hpp>

#include <stdexcept>

namespace fusion {

InFlightManager::InFlightManager(Device& device,
                                 uint32_t framesInFlight)
    : m_deviceRef(device),
      m_framesInFlight(framesInFlight)
{
    createFrames();
}

InFlightManager::~InFlightManager() {
    VkDevice device = m_deviceRef.vkDevice();

    for (auto& frame : m_frames) {
        vkDestroySemaphore(device,
                           frame->imageAvailable(),
                           nullptr);
        vkDestroySemaphore(device,
                           frame->renderFinished(),
                           nullptr);
        vkDestroyFence(device,
                       frame->fence(),
                       nullptr);
    }
}

FrameContext& InFlightManager::currentFrame() {
    return *m_frames[m_currentFrame];
}

void InFlightManager::advanceFrame() {
    m_currentFrame =
        (m_currentFrame + 1) % m_framesInFlight;
}

void InFlightManager::createFrames() {
    VkDevice device = m_deviceRef.vkDevice();

    for (uint32_t i = 0; i < m_framesInFlight; i++) {

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex =
            m_deviceRef.graphicsQueueFamily();
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        VkCommandPool pool;
        vkCreateCommandPool(device,
                            &poolInfo,
                            nullptr,
                            &pool);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = pool;
        allocInfo.level =
            VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer cmd;
        vkAllocateCommandBuffers(device,
                                 &allocInfo,
                                 &cmd);

        VkSemaphoreCreateInfo semInfo{};
        semInfo.sType =
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType =
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags =
            VK_FENCE_CREATE_SIGNALED_BIT;

        VkSemaphore imageAvailable;
        VkSemaphore renderFinished;
        VkFence fence;

        vkCreateSemaphore(device,
                          &semInfo,
                          nullptr,
                          &imageAvailable);
        vkCreateSemaphore(device,
                          &semInfo,
                          nullptr,
                          &renderFinished);
        vkCreateFence(device,
                      &fenceInfo,
                      nullptr,
                      &fence);

        m_frames.push_back(
            std::make_unique<FrameContext>(
                m_deviceRef,
                pool,
                cmd,
                fence,
                imageAvailable,
                renderFinished));
    }
}

}