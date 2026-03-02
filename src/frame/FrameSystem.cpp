#include <fusion/frame/FrameSystem.hpp>
#include <fusion/frame/InFlightManager.hpp>
#include <fusion/frame/FrameContext.hpp>

#include <fusion/core/Device.hpp>
#include <fusion/core/Surface.hpp>
#include <fusion/core/Swapchain.hpp>

#include <stdexcept>

namespace fusion {

FrameSystem::FrameSystem(Device& device,
                         Surface& surface)
    : m_deviceRef(device),
      m_surfaceRef(surface)
{
    // Default swapchain size — will be recreated properly later
    createSwapchain();

    m_inFlight = std::make_unique<InFlightManager>(
        m_deviceRef,
        3);
}

FrameSystem::~FrameSystem() {
    vkDeviceWaitIdle(m_deviceRef.vkDevice());
}

void FrameSystem::createSwapchain() {
    // Temporary default size — user should recreate on window resize
    uint32_t width = 1280;
    uint32_t height = 720;

    m_swapchain = std::make_unique<Swapchain>(
        m_deviceRef,
        m_surfaceRef,
        width,
        height);
}

void FrameSystem::recreateSwapchain() {
    vkDeviceWaitIdle(m_deviceRef.vkDevice());

    uint32_t width = m_swapchain->extent().width;
    uint32_t height = m_swapchain->extent().height;

    m_swapchain->recreate(width, height);
}

FrameContext& FrameSystem::beginFrame() {
    FrameContext& frame = m_inFlight->currentFrame();

    VkDevice device = m_deviceRef.vkDevice();

    // Wait for previous frame
    vkWaitForFences(device,
                    1,
                    &frame.fence(),
                    VK_TRUE,
                    UINT64_MAX);

    vkResetFences(device,
                  1,
                  &frame.fence());

    // Acquire next image
    m_currentImageIndex =
        m_swapchain->acquireNextImage(
            frame.imageAvailable());

    frame.reset();

    VkCommandBuffer cmd = frame.commandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags =
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmd, &beginInfo);

    return frame;
}

void FrameSystem::endFrame() {
    FrameContext& frame = m_inFlight->currentFrame();
    VkCommandBuffer cmd = frame.commandBuffer();

    vkEndCommandBuffer(cmd);

    VkSemaphore waitSemaphores[] = {
        frame.imageAvailable()
    };

    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSemaphore signalSemaphores[] = {
        frame.renderFinished()
    };

    VkSubmitInfo submitInfo{};
    submitInfo.sType =
        VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(
            m_deviceRef.graphicsQueue(),
            1,
            &submitInfo,
            frame.fence()) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to submit draw command buffer");
    }

    m_swapchain->present(
        m_currentImageIndex,
        frame.renderFinished());

    m_inFlight->advanceFrame();
}

uint32_t FrameSystem::currentFrameIndex() const {
    return m_inFlight->frameIndex();
}

}