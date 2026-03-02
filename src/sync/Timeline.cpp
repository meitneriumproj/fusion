#include <fusion/sync/Timeline.hpp>
#include <fusion/core/Device.hpp>

#include <stdexcept>

namespace fusion {

Timeline::Timeline(Device& device,
                   uint64_t initialValue)
    : m_deviceRef(device)
{
    VkSemaphoreTypeCreateInfo timelineInfo{};
    timelineInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    timelineInfo.semaphoreType =
        VK_SEMAPHORE_TYPE_TIMELINE;
    timelineInfo.initialValue = initialValue;

    VkSemaphoreCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.pNext = &timelineInfo;

    if (vkCreateSemaphore(
            m_deviceRef.vkDevice(),
            &createInfo,
            nullptr,
            &m_semaphore) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create timeline semaphore");
    }
}

Timeline::~Timeline() {
    if (m_semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(
            m_deviceRef.vkDevice(),
            m_semaphore,
            nullptr);
    }
}

Timeline::Timeline(Timeline&& other) noexcept
    : m_deviceRef(other.m_deviceRef),
      m_semaphore(other.m_semaphore)
{
    other.m_semaphore = VK_NULL_HANDLE;
}

Timeline& Timeline::operator=(Timeline&& other) noexcept {
    if (this != &other) {
        m_semaphore = other.m_semaphore;
        other.m_semaphore = VK_NULL_HANDLE;
    }
    return *this;
}

void Timeline::signal(uint64_t value) {
    VkSemaphoreSignalInfo signalInfo{};
    signalInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
    signalInfo.semaphore = m_semaphore;
    signalInfo.value = value;

    vkSignalSemaphore(
        m_deviceRef.vkDevice(),
        &signalInfo);
}

void Timeline::wait(uint64_t value) {
    VkSemaphoreWaitInfo waitInfo{};
    waitInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
    waitInfo.semaphoreCount = 1;
    waitInfo.pSemaphores = &m_semaphore;
    waitInfo.pValues = &value;

    vkWaitSemaphores(
        m_deviceRef.vkDevice(),
        &waitInfo,
        UINT64_MAX);
}

uint64_t Timeline::value() const {
    uint64_t val;
    vkGetSemaphoreCounterValue(
        m_deviceRef.vkDevice(),
        m_semaphore,
        &val);
    return val;
}

}