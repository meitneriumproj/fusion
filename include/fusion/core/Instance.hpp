// Copyright 2024-2026 The Franchuk Project authors.
// All rights reserved.
#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace fusion {

struct InstanceCreateInfo {
    bool enableValidation = true;
    std::vector<const char*> requiredExtensions;
};

class Instance {
public:
    explicit Instance(const InstanceCreateInfo& createInfo);
    ~Instance();

    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    Instance(Instance&& other) noexcept;
    Instance& operator=(Instance&& other) noexcept;

    VkInstance vkInstance() const noexcept { return m_instance; }

private:
    void createInstance(const InstanceCreateInfo& createInfo);
    void setupDebugMessenger();
    void destroyDebugMessenger();

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    bool m_validationEnabled = false;
};

}