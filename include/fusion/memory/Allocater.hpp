#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;

class Allocator {
public:
    explicit Allocator(Device& device);
    ~Allocator();

    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;

    VkDeviceMemory allocate(const VkMemoryRequirements& requirements,
                            VkMemoryPropertyFlags properties);

    void free(VkDeviceMemory memory);

private:
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);

    Device& m_deviceRef;
};

}