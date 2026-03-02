#include <fusion/sync/BarrierBuilder.hpp>

namespace fusion {

BarrierBuilder& BarrierBuilder::srcStage(
    VkPipelineStageFlags stage)
{
    m_srcStage = stage;
    return *this;
}

BarrierBuilder& BarrierBuilder::dstStage(
    VkPipelineStageFlags stage)
{
    m_dstStage = stage;
    return *this;
}

BarrierBuilder& BarrierBuilder::srcAccess(
    VkAccessFlags access)
{
    m_srcAccess = access;
    return *this;
}

BarrierBuilder& BarrierBuilder::dstAccess(
    VkAccessFlags access)
{
    m_dstAccess = access;
    return *this;
}

void BarrierBuilder::apply(VkCommandBuffer cmd)
{
    VkMemoryBarrier barrier{};
    barrier.sType =
        VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    barrier.srcAccessMask = m_srcAccess;
    barrier.dstAccessMask = m_dstAccess;

    vkCmdPipelineBarrier(
        cmd,
        m_srcStage,
        m_dstStage,
        0,
        1,
        &barrier,
        0,
        nullptr,
        0,
        nullptr);
}

}