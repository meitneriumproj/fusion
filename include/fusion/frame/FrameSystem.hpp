#pragma once

#include <memory>
#include <fusion/core/Swapchain.hpp>

namespace fusion {

class Device;
class Surface;
class InFlightManager;
class FrameContext;

class FrameSystem {
public:
    FrameSystem(Device& device, Surface& surface);
    ~FrameSystem();

    FrameSystem(const FrameSystem&) = delete;
    FrameSystem& operator=(const FrameSystem&) = delete;

    FrameContext& beginFrame();
    void endFrame();

    uint32_t currentFrameIndex() const;
    std::unique_ptr<Swapchain> m_swapchain;
    uint32_t m_currentImageIndex = 0;

private:
    void createSwapchain();
    void recreateSwapchain();

    Device& m_deviceRef;
    Surface& m_surfaceRef;

    std::unique_ptr<InFlightManager> m_inFlight;

    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    VkFormat m_swapchainFormat;
    VkExtent2D m_swapchainExtent;
};

}