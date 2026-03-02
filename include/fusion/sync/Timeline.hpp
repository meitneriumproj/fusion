#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;

class Timeline {
public:
    explicit Timeline(Device& device, uint64_t initialValue = 0);
    ~Timeline();

    Timeline(const Timeline&) = delete;
    Timeline& operator=(const Timeline&) = delete;

    Timeline(Timeline&& other) noexcept;
    Timeline& operator=(Timeline&& other) noexcept;

    void signal(uint64_t value);
    void wait(uint64_t value);

    uint64_t value() const;

private:
    Device& m_deviceRef;
    VkSemaphore m_semaphore = VK_NULL_HANDLE;
};

}