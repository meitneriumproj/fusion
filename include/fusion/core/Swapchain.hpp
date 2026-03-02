#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace fusion {

class Device;
class Surface;

class Swapchain {
public:
    Swapchain(Device& device,
              Surface& surface,
              uint32_t width,
              uint32_t height);
    ~Swapchain();

    Swapchain(const Swapchain&) = delete;
    Swapchain& operator=(const Swapchain&) = delete;

    Swapchain(Swapchain&& other) noexcept;
    Swapchain& operator=(Swapchain&& other) noexcept;

    void recreate(uint32_t width, uint32_t height);

    VkSwapchainKHR vkSwapchain() const noexcept { return m_swapchain; }
    VkFormat format() const noexcept { return m_format; }
    VkExtent2D extent() const noexcept { return m_extent; }

    const std::vector<VkImageView>& imageViews() const noexcept {
        return m_imageViews;
    }

    uint32_t acquireNextImage(VkSemaphore imageAvailable);
    void present(uint32_t imageIndex,
                 VkSemaphore renderFinished);

private:
    void createSwapchain(uint32_t width, uint32_t height);
    void createImageViews();
    void destroy();

    Device& m_deviceRef;
    Surface& m_surfaceRef;

    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    VkFormat m_format{};
    VkExtent2D m_extent{};

    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;
};

}