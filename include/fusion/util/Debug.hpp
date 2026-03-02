#pragma once

#include <vulkan/vulkan.h>
#include <string>

namespace fusion {

class Device;

class Debug {
public:
    static void beginLabel(VkCommandBuffer cmd,
                           const std::string& name,
                           float r = 0.5f,
                           float g = 0.5f,
                           float b = 0.5f);

    static void endLabel(VkCommandBuffer cmd);

    static void setObjectName(Device& device,
                              uint64_t handle,
                              VkObjectType type,
                              const std::string& name);
};

}