#include <fusion/core/Device.hpp>
#include <fusion/core/Instance.hpp>
#include <fusion/core/Surface.hpp>

#include <vector>
#include <stdexcept>

namespace fusion {

Device::Device(Instance& instance,
               Surface* surface)
    : m_instanceRef(instance),
      m_surface(surface)
{
    pickPhysicalDevice();
    createLogicalDevice();
}

Device::~Device() {
    if (m_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device, nullptr);
    }
}

Device::Device(Device&& other) noexcept
    : m_instanceRef(other.m_instanceRef),
      m_surface(other.m_surface),
      m_physicalDevice(other.m_physicalDevice),
      m_device(other.m_device),
      m_graphicsQueue(other.m_graphicsQueue),
      m_presentQueue(other.m_presentQueue),
      m_queueFamilies(other.m_queueFamilies)
{
    other.m_device = VK_NULL_HANDLE;
}

Device& Device::operator=(Device&& other) noexcept {
    if (this != &other) {
        if (m_device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_device, nullptr);
        }

        m_physicalDevice = other.m_physicalDevice;
        m_device = other.m_device;
        m_graphicsQueue = other.m_graphicsQueue;
        m_presentQueue = other.m_presentQueue;
        m_queueFamilies = other.m_queueFamilies;

        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

void Device::pickPhysicalDevice() {
    VkInstance instance = m_instanceRef.vkInstance();

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("No Vulkan devices found");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        auto indices = findQueueFamilies(device);
        if (indices.complete()) {
            m_physicalDevice = device;
            m_queueFamilies = indices;
            return;
        }
    }

    throw std::runtime_error("Failed to find suitable GPU");
}

QueueFamilyIndices Device::findQueueFamilies(
    VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueCount, nullptr);

    std::vector<VkQueueFamilyProperties> families(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueCount, families.data());

    for (uint32_t i = 0; i < queueCount; i++) {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
        }

        if (m_surface) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(
                device,
                i,
                m_surface->vkSurface(),
                &presentSupport);

            if (presentSupport) {
                indices.present = i;
            }
        } else {
            indices.present = indices.graphics;
        }
    }

    return indices;
}

void Device::createLogicalDevice() {
    float priority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    std::vector<uint32_t> uniqueQueues = {
        m_queueFamilies.graphics.value()
    };

    if (m_queueFamilies.present !=
        m_queueFamilies.graphics) {
        uniqueQueues.push_back(
            m_queueFamilies.present.value());
    }

    for (uint32_t queueFamily : uniqueQueues) {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType =
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamily;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &priority;
        queueInfos.push_back(queueInfo);
    }

    VkPhysicalDeviceFeatures features{};

    const char* extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueInfos.size());
    createInfo.pQueueCreateInfos =
        queueInfos.data();
    createInfo.pEnabledFeatures = &features;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateDevice(m_physicalDevice,
                       &createInfo,
                       nullptr,
                       &m_device) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create logical device");
    }

    vkGetDeviceQueue(m_device,
                     m_queueFamilies.graphics.value(),
                     0,
                     &m_graphicsQueue);

    vkGetDeviceQueue(m_device,
                     m_queueFamilies.present.value(),
                     0,
                     &m_presentQueue);
}

}