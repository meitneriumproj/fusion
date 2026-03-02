#pragma once

#include <vector>
#include <memory>
#include <string>

#include <fusion/graph/PassBuilder.hpp>
#include <fusion/resource/ResourceDescription.hpp>
#include <fusion/graph/ResourceHandle.hpp>

namespace fusion {

class Device;
class FrameContext;
class Texture;
class Buffer;
class DependencyResolver;

class RenderGraph {
public:
    explicit RenderGraph(Device& device);
    ~RenderGraph();

    RenderGraph(const RenderGraph&) = delete;
    RenderGraph& operator=(const RenderGraph&) = delete;

    ResourceHandle createTexture(const TextureDescription& desc);
    ResourceHandle createBuffer(const BufferDescription& desc);

    PassBuilder addPass(const std::string& name);

    void compile();
    void execute(FrameContext& frame);

private:
    Device& m_deviceRef;

    std::vector<TextureDescription> m_textureDescs;
    std::vector<BufferDescription> m_bufferDescs;

    std::vector<std::unique_ptr<Texture>> m_textures;
    std::vector<std::unique_ptr<Buffer>> m_buffers;

    std::unique_ptr<DependencyResolver> m_resolver;
    std::vector<InternalPass> m_internalPasses;
    void registerPass(const std::string&,
                      const std::vector<ResourceHandle>&,
                      const std::vector<ResourceHandle>&,
                      PassExecuteFn);
};

}