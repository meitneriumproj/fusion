#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class Device;
class ResourceHandle;

class PassContext {
public:
    PassContext(Device& device,
                VkCommandBuffer cmd);

    VkCommandBuffer commandBuffer() const noexcept { return m_cmd; }

    void bindPipeline(VkPipeline pipeline);
    void draw(uint32_t vertexCount,
              uint32_t instanceCount = 1,
              uint32_t firstVertex = 0,
              uint32_t firstInstance = 0);

private:
    Device& m_deviceRef;
    VkCommandBuffer m_cmd;
};

}