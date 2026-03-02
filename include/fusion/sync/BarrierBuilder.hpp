#pragma once

#include <vulkan/vulkan.h>

namespace fusion {

class BarrierBuilder {
public:
    BarrierBuilder& srcStage(VkPipelineStageFlags stage);
    BarrierBuilder& dstStage(VkPipelineStageFlags stage);
    BarrierBuilder& srcAccess(VkAccessFlags access);
    BarrierBuilder& dstAccess(VkAccessFlags access);

    void apply(VkCommandBuffer cmd);

private:
    VkPipelineStageFlags m_srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkPipelineStageFlags m_dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    VkAccessFlags m_srcAccess = 0;
    VkAccessFlags m_dstAccess = 0;
};

}