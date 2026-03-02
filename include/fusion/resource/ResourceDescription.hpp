#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

struct TextureDescription {
    uint32_t width = 1;
    uint32_t height = 1;
    uint32_t depth = 1;

    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;

    bool transient = false;
};

struct BufferDescription {
    VkDeviceSize size = 0;
    VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bool transient = false;
};

}