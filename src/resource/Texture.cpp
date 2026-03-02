#include <fusion/resource/Texture.hpp>
#include <fusion/core/Device.hpp>
#include <fusion/memory/Allocator.hpp>

#include <stdexcept>

namespace fusion {

Texture::Texture(Device& device,
                 const TextureDescription& desc)
    : m_deviceRef(device),
      m_desc(desc)
{
    createImage();
    createView();
}

Texture::~Texture() {
    destroy();
}

Texture::Texture(Texture&& other) noexcept
    : m_deviceRef(other.m_deviceRef),
      m_desc(other.m_desc),
      m_image(other.m_image),
      m_view(other.m_view),
      m_memory(other.m_memory)
{
    other.m_image = VK_NULL_HANDLE;
    other.m_view = VK_NULL_HANDLE;
    other.m_memory = VK_NULL_HANDLE;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        destroy();

        m_image = other.m_image;
        m_view = other.m_view;
        m_memory = other.m_memory;
        m_desc = other.m_desc;

        other.m_image = VK_NULL_HANDLE;
        other.m_view = VK_NULL_HANDLE;
        other.m_memory = VK_NULL_HANDLE;
    }
    return *this;
}

void Texture::createImage() {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = m_desc.width;
    imageInfo.extent.height = m_desc.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = m_desc.format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = m_desc.usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_deviceRef.vkDevice(),
                      &imageInfo,
                      nullptr,
                      &m_image) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create image");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(
        m_deviceRef.vkDevice(),
        m_image,
        &memRequirements);

    Allocator allocator(m_deviceRef);

    m_memory = allocator.allocate(
        memRequirements,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vkBindImageMemory(
        m_deviceRef.vkDevice(),
        m_image,
        m_memory,
        0);
}

void Texture::createView() {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType =
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = m_desc.format;
    viewInfo.subresourceRange.aspectMask =
        m_desc.aspect;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(
            m_deviceRef.vkDevice(),
            &viewInfo,
            nullptr,
            &m_view) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create image view");
    }
}

void Texture::destroy() {
    VkDevice device = m_deviceRef.vkDevice();

    if (m_view != VK_NULL_HANDLE) {
        vkDestroyImageView(device,
                           m_view,
                           nullptr);
        m_view = VK_NULL_HANDLE;
    }

    if (m_image != VK_NULL_HANDLE) {
        vkDestroyImage(device,
                       m_image,
                       nullptr);
        m_image = VK_NULL_HANDLE;
    }

    if (m_memory != VK_NULL_HANDLE) {
        vkFreeMemory(device,
                     m_memory,
                     nullptr);
        m_memory = VK_NULL_HANDLE;
    }
}

}