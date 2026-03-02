#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;

class Sampler {
public:
    Sampler(Device& device);
    ~Sampler();

    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;

    Sampler(Sampler&& other) noexcept;
    Sampler& operator=(Sampler&& other) noexcept;

    VkSampler vkSampler() const noexcept { return m_sampler; }

private:
    Device& m_deviceRef;
    VkSampler m_sampler = VK_NULL_HANDLE;
};

}