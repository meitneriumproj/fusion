#pragma once

#include <vulkan/vulkan.h>
#include <fusion/resource/ResourceDescription.hpp>

namespace fusion {

class Device;

class Buffer {
public:
    Buffer(Device& device, const BufferDescription& desc);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    VkBuffer buffer() const noexcept { return m_buffer; }
    VkDeviceSize size() const noexcept { return m_desc.size; }

private:
    void createBuffer();
    void destroy();

    Device& m_deviceRef;
    BufferDescription m_desc;

    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
};

}