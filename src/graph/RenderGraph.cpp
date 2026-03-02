#include <fusion/graph/RenderGraph.hpp>
#include <fusion/graph/DependencyResolver.hpp>
#include <fusion/graph/PassContext.hpp>
#include <fusion/frame/FrameContext.hpp>
#include <fusion/core/Device.hpp>

namespace fusion {

struct InternalPass {
    std::string name;
    std::vector<ResourceHandle> reads;
    std::vector<ResourceHandle> writes;
    PassExecuteFn execute;
};

RenderGraph::RenderGraph(Device& device)
    : m_deviceRef(device),
      m_resolver(std::make_unique<DependencyResolver>())
{
}

RenderGraph::~RenderGraph() = default;

PassBuilder RenderGraph::addPass(
    const std::string& name)
{
    return PassBuilder(*this, name);
}

void RenderGraph::registerPass(
    const std::string& name,
    const std::vector<ResourceHandle>& reads,
    const std::vector<ResourceHandle>& writes,
    PassExecuteFn fn)
{
    InternalPass pass{ name, reads, writes, fn };
    m_internalPasses.push_back(pass);

    PassNode node{ reads, writes };
    m_resolver->addPass(node);
}

void RenderGraph::compile()
{
    m_resolver->resolve();
}

void RenderGraph::execute(FrameContext& frame)
{
    VkCommandBuffer cmd = frame.commandBuffer();

    for (auto& pass : m_internalPasses) {
        PassContext context(m_deviceRef, cmd);
        pass.execute(context);
    }

    m_internalPasses.clear();
}

}