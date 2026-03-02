// Copyright 2024-2026 The Franchuk Project authors.
// All rights reserved.

#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Instance;

class Surface {
public:
    Surface(Instance& instance, VkSurfaceKHR surface);
    ~Surface();

    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    Surface(Surface&& other) noexcept;
    Surface& operator=(Surface&& other) noexcept;

    VkSurfaceKHR vkSurface() const noexcept { return m_surface; }

private:
    Instance& m_instanceRef;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
};

}