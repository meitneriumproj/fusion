#include <fusion/resource/Buffer.hpp>
#include <fusion/core/Device.hpp>
#include <fusion/memory/Allocator.hpp>

#include <stdexcept>

namespace fusion {

Buffer::Buffer(Device& device,
               const BufferDescription& desc)
    : m_deviceRef(device),
      m_desc(desc)
{
    createBuffer();
}

Buffer::~Buffer() {
    destroy();
}

Buffer::Buffer(Buffer&& other) noexcept
    : m_deviceRef(other.m_deviceRef),
      m_desc(other.m_desc),
      m_buffer(other.m_buffer),
      m_memory(other.m_memory)
{
    other.m_buffer = VK_NULL_HANDLE;
    other.m_memory = VK_NULL_HANDLE;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        destroy();

        m_buffer = other.m_buffer;
        m_memory = other.m_memory;
        m_desc = other.m_desc;

        other.m_buffer = VK_NULL_HANDLE;
        other.m_memory = VK_NULL_HANDLE;
    }
    return *this;
}

void Buffer::createBuffer() {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_desc.size;
    bufferInfo.usage = m_desc.usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_deviceRef.vkDevice(),
                       &bufferInfo,
                       nullptr,
                       &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(
        m_deviceRef.vkDevice(),
        m_buffer,
        &memRequirements);

    Allocator allocator(m_deviceRef);

    m_memory = allocator.allocate(
        memRequirements,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vkBindBufferMemory(
        m_deviceRef.vkDevice(),
        m_buffer,
        m_memory,
        0);
}

void Buffer::destroy() {
    VkDevice device = m_deviceRef.vkDevice();

    if (m_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(device, m_buffer, nullptr);
        m_buffer = VK_NULL_HANDLE;
    }

    if (m_memory != VK_NULL_HANDLE) {
        vkFreeMemory(device, m_memory, nullptr);
        m_memory = VK_NULL_HANDLE;
    }
}

}