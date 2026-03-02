#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace fusion {

class Device;

class MemoryPool {
public:
    MemoryPool(Device& device,
               VkDeviceSize blockSize,
               VkMemoryPropertyFlags properties);
    ~MemoryPool();

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    VkDeviceMemory allocate(VkDeviceSize size, VkDeviceSize& offset);

    void reset();

private:
    Device& m_deviceRef;

    VkDeviceSize m_blockSize;
    VkMemoryPropertyFlags m_properties;

    std::vector<VkDeviceMemory> m_blocks;
    VkDeviceSize m_currentOffset = 0;
};

}