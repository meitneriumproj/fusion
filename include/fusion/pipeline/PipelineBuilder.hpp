#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace fusion {

class Device;
class GraphicsPipeline;
class ComputePipeline;

class PipelineBuilder {
public:
    explicit PipelineBuilder(Device& device);

    // Shader stages
    PipelineBuilder& vertexShader(const std::string& path);
    PipelineBuilder& fragmentShader(const std::string& path);
    PipelineBuilder& computeShader(const std::string& path);

    // Fixed function
    PipelineBuilder& viewport(float width, float height);
    PipelineBuilder& depthTest(bool enable);
    PipelineBuilder& blendAlpha();

    GraphicsPipeline buildGraphics(VkRenderPass renderPass);
    ComputePipeline buildCompute();

private:
    Device& m_deviceRef;

    std::string m_vertPath;
    std::string m_fragPath;
    std::string m_compPath;

    bool m_depthTest = false;
    bool m_blendAlpha = false;

    float m_viewportWidth = 0.f;
    float m_viewportHeight = 0.f;
};

}