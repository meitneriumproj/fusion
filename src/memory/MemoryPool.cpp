#include <fusion/memory/MemoryPool.hpp>
#include <fusion/core/Device.hpp>

#include <stdexcept>

namespace fusion {

MemoryPool::MemoryPool(Device& device,
                       VkDeviceSize blockSize,
                       VkMemoryPropertyFlags properties)
    : m_deviceRef(device),
      m_blockSize(blockSize),
      m_properties(properties)
{
}

MemoryPool::~MemoryPool() {
    VkDevice device = m_deviceRef.vkDevice();

    for (auto mem : m_blocks) {
        vkFreeMemory(device, mem, nullptr);
    }
}

VkDeviceMemory MemoryPool::allocate(VkDeviceSize size,
                                    VkDeviceSize& offset)
{
    if (m_blocks.empty() ||
        m_currentOffset + size > m_blockSize) {

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType =
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = m_blockSize;

        // Simple memory type selection (device local for now)
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(
            m_deviceRef.physicalDevice(),
            &memProps);

        uint32_t memoryType = 0;
        for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
            if (memProps.memoryTypes[i].propertyFlags &
                m_properties) {
                memoryType = i;
                break;
            }
        }

        allocInfo.memoryTypeIndex = memoryType;

        VkDeviceMemory memory;
        if (vkAllocateMemory(
                m_deviceRef.vkDevice(),
                &allocInfo,
                nullptr,
                &memory) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to allocate memory pool block");
        }

        m_blocks.push_back(memory);
        m_currentOffset = 0;
    }

    offset = m_currentOffset;
    m_currentOffset += size;

    return m_blocks.back();
}

void MemoryPool::reset() {
    m_currentOffset = 0;
}

}