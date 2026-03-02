#include <fusion/fusion.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

static VkRenderPass createRenderPass(fusion::Device& device,
                                     VkFormat format)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorRef;

    VkRenderPassCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &colorAttachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;

    VkRenderPass renderPass;
    if (vkCreateRenderPass(device.vkDevice(),
                           &info,
                           nullptr,
                           &renderPass) != VK_SUCCESS)
        throw std::runtime_error("RenderPass failed");

    return renderPass;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window =
        glfwCreateWindow(1280, 720, "Fusion Triangle", nullptr, nullptr);

    // --- Instance ---
    uint32_t extCount = 0;
    const char** glfwExt =
        glfwGetRequiredInstanceExtensions(&extCount);

    fusion::InstanceCreateInfo instanceInfo{};
    for (uint32_t i = 0; i < extCount; i++)
        instanceInfo.requiredExtensions.push_back(glfwExt[i]);

    fusion::Instance instance(instanceInfo);

    // --- Surface ---
    VkSurfaceKHR surfaceRaw;
    glfwCreateWindowSurface(instance.vkInstance(),
                            window,
                            nullptr,
                            &surfaceRaw);

    fusion::Surface surface(instance, surfaceRaw);

    // --- Device ---
    fusion::Device device(instance, &surface);

    // --- Frame System ---
    fusion::FrameSystem frameSystem(device, surface);

    // --- RenderPass ---
    VkRenderPass renderPass =
        createRenderPass(device,
                         frameSystem.swapchain().format());

    // --- Pipeline ---
    fusion::PipelineBuilder builder(device);

    auto pipeline =
        builder.vertexShader("shaders/triangle.vert.spv")
               .fragmentShader("shaders/triangle.frag.spv")
               .viewport(1280, 720)
               .buildGraphics(renderPass);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        auto& frame = frameSystem.beginFrame();
        VkCommandBuffer cmd = frame.commandBuffer();

        VkClearValue clear{};
        clear.color = {0.1f, 0.1f, 0.15f, 1.0f};

        VkRenderPassBeginInfo rpInfo{};
        rpInfo.sType =
            VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpInfo.renderPass = renderPass;
        rpInfo.framebuffer = VK_NULL_HANDLE; // TODO: framebuffer
        rpInfo.renderArea.extent = {1280, 720};
        rpInfo.clearValueCount = 1;
        rpInfo.pClearValues = &clear;

        vkCmdBeginRenderPass(cmd,
                             &rpInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmd,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline.vkPipeline());

        vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdEndRenderPass(cmd);

        frameSystem.endFrame();
    }

    vkDeviceWaitIdle(device.vkDevice());
}