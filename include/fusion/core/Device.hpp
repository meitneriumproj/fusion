// Copyright 2024-2026 The Franchuk Project authors.
// All rights reserved.

#pragma once

#include <vulkan/vulkan.h>
#include <optional>

namespace fusion {

class Instance;
class Surface;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;

    bool complete() const {
        return graphics.has_value() && present.has_value();
    }
};

class Device {
public:
    Device(Instance& instance, Surface* surface = nullptr);
    ~Device();

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    Device(Device&& other) noexcept;
    Device& operator=(Device&& other) noexcept;

    VkDevice vkDevice() const noexcept { return m_device; }
    VkPhysicalDevice physicalDevice() const noexcept { return m_physicalDevice; }

    VkQueue graphicsQueue() const noexcept { return m_graphicsQueue; }
    VkQueue presentQueue() const noexcept { return m_presentQueue; }

    uint32_t graphicsQueueFamily() const noexcept { return m_queueFamilies.graphics.value(); }
    uint32_t presentQueueFamily() const noexcept { return m_queueFamilies.present.value(); }

private:
    void pickPhysicalDevice();
    void createLogicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    Instance& m_instanceRef;
    Surface* m_surface;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;

    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    QueueFamilyIndices m_queueFamilies;
};

}