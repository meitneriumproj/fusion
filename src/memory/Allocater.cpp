#include <fusion/memory/Allocator.hpp>
#include <fusion/core/Device.hpp>

#include <stdexcept>

namespace fusion {

Allocator::Allocator(Device& device)
    : m_deviceRef(device)
{
}

Allocator::~Allocator() = default;

VkDeviceMemory Allocator::allocate(
    const VkMemoryRequirements& requirements,
    VkMemoryPropertyFlags properties)
{
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex =
        findMemoryType(requirements.memoryTypeBits,
                       properties);

    VkDeviceMemory memory;

    if (vkAllocateMemory(m_deviceRef.vkDevice(),
                         &allocInfo,
                         nullptr,
                         &memory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate memory");
    }

    return memory;
}

void Allocator::free(VkDeviceMemory memory) {
    if (memory != VK_NULL_HANDLE) {
        vkFreeMemory(m_deviceRef.vkDevice(),
                     memory,
                     nullptr);
    }
}

uint32_t Allocator::findMemoryType(
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(
        m_deviceRef.physicalDevice(),
        &memProperties);

    for (uint32_t i = 0;
         i < memProperties.memoryTypeCount;
         i++) {

        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags &
             properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error(
        "ERR! Failed to find suitable memory type");
}

}