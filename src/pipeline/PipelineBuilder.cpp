#include <fusion/pipeline/PipelineBuilder.hpp>
#include <fusion/pipeline/GraphicsPipeline.hpp>
#include <fusion/pipeline/ComputePipeline.hpp>
#include <fusion/core/Device.hpp>

#include <fstream>
#include <vector>
#include <stdexcept>

namespace fusion {

static std::vector<char> readFile(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("Failed to open shader file");

    size_t size = (size_t)file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);

    return buffer;
}

static VkShaderModule createShaderModule(
    VkDevice device,
    const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType =
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode =
        reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule module;

    if (vkCreateShaderModule(device,
                             &createInfo,
                             nullptr,
                             &module) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create shader module");
    }

    return module;
}

PipelineBuilder::PipelineBuilder(Device& device)
    : m_deviceRef(device)
{
}

PipelineBuilder& PipelineBuilder::vertexShader(
    const std::string& path)
{
    m_vertPath = path;
    return *this;
}

PipelineBuilder& PipelineBuilder::fragmentShader(
    const std::string& path)
{
    m_fragPath = path;
    return *this;
}

PipelineBuilder& PipelineBuilder::computeShader(
    const std::string& path)
{
    m_compPath = path;
    return *this;
}

PipelineBuilder& PipelineBuilder::viewport(
    float width, float height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
    return *this;
}

PipelineBuilder& PipelineBuilder::depthTest(bool enable)
{
    m_depthTest = enable;
    return *this;
}

PipelineBuilder& PipelineBuilder::blendAlpha()
{
    m_blendAlpha = true;
    return *this;
}

GraphicsPipeline PipelineBuilder::buildGraphics(
    VkRenderPass renderPass)
{
    auto vertCode = readFile(m_vertPath);
    auto fragCode = readFile(m_fragPath);

    VkDevice device = m_deviceRef.vkDevice();

    VkShaderModule vertModule =
        createShaderModule(device, vertCode);
    VkShaderModule fragModule =
        createShaderModule(device, fragCode);

    VkPipelineShaderStageCreateInfo stages[2]{};

    stages[0].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = vertModule;
    stages[0].pName = "main";

    stages[1].sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = fragModule;
    stages[1].pName = "main";

    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology =
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport viewport{};
    viewport.width = m_viewportWidth;
    viewport.height = m_viewportHeight;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.extent.width = (uint32_t)m_viewportWidth;
    scissor.extent.height = (uint32_t)m_viewportHeight;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples =
        VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlend{};
    colorBlend.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    if (m_blendAlpha) {
        colorBlend.blendEnable = VK_TRUE;
        colorBlend.srcColorBlendFactor =
            VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlend.dstColorBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlend.colorBlendOp =
            VK_BLEND_OP_ADD;
        colorBlend.srcAlphaBlendFactor =
            VK_BLEND_FACTOR_ONE;
        colorBlend.dstAlphaBlendFactor =
            VK_BLEND_FACTOR_ZERO;
        colorBlend.alphaBlendOp =
            VK_BLEND_OP_ADD;
    }

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlend;

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    VkPipelineLayout layout;
    vkCreatePipelineLayout(device,
                           &layoutInfo,
                           nullptr,
                           &layout);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType =
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages;
    pipelineInfo.pVertexInputState = &vertexInput;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;

    VkPipeline pipeline;
    if (vkCreateGraphicsPipelines(device,
                                   VK_NULL_HANDLE,
                                   1,
                                   &pipelineInfo,
                                   nullptr,
                                   &pipeline) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create graphics pipeline");
    }

    vkDestroyShaderModule(device, vertModule, nullptr);
    vkDestroyShaderModule(device, fragModule, nullptr);

    return GraphicsPipeline(
        m_deviceRef,
        pipeline,
        layout);
}

ComputePipeline PipelineBuilder::buildCompute()
{
    auto compCode = readFile(m_compPath);

    VkDevice device = m_deviceRef.vkDevice();

    VkShaderModule compModule =
        createShaderModule(device, compCode);

    VkPipelineShaderStageCreateInfo stage{};
    stage.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    stage.module = compModule;
    stage.pName = "main";

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    VkPipelineLayout layout;
    vkCreatePipelineLayout(device,
                           &layoutInfo,
                           nullptr,
                           &layout);

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType =
        VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = stage;
    pipelineInfo.layout = layout;

    VkPipeline pipeline;
    if (vkCreateComputePipelines(device,
                                 VK_NULL_HANDLE,
                                 1,
                                 &pipelineInfo,
                                 nullptr,
                                 &pipeline) != VK_SUCCESS) {
        throw std::runtime_error(
            "Failed to create compute pipeline");
    }

    vkDestroyShaderModule(device, compModule, nullptr);

    return ComputePipeline(
        m_deviceRef,
        pipeline,
        layout);
}

}