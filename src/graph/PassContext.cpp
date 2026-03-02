#include <fusion/graph/PassContext.hpp>
#include <fusion/core/Device.hpp>

namespace fusion {

PassContext::PassContext(Device& device,
                         VkCommandBuffer cmd)
    : m_deviceRef(device),
      m_cmd(cmd)
{
}

void PassContext::bindPipeline(VkPipeline pipeline) {
    vkCmdBindPipeline(
        m_cmd,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline);
}

void PassContext::draw(uint32_t vertexCount,
                       uint32_t instanceCount,
                       uint32_t firstVertex,
                       uint32_t firstInstance)
{
    vkCmdDraw(m_cmd,
              vertexCount,
              instanceCount,
              firstVertex,
              firstInstance);
}

}