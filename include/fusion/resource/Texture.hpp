#pragma once

#include <vulkan/vulkan.h>
#include <fusion/resource/ResourceDescription.hpp>

namespace fusion {

class Device;

class Texture {
public:
    Texture(Device& device, const TextureDescription& desc);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    VkImage image() const noexcept { return m_image; }
    VkImageView view() const noexcept { return m_view; }
    VkFormat format() const noexcept { return m_desc.format; }

private:
    void createImage();
    void createView();
    void destroy();

    Device& m_deviceRef;
    TextureDescription m_desc;

    VkImage m_image = VK_NULL_HANDLE;
    VkImageView m_view = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
};

}