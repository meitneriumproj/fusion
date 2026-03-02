#include <fusion/graph/PassBuilder.hpp>
#include <fusion/graph/RenderGraph.hpp>

namespace fusion {

PassBuilder::PassBuilder(RenderGraph& graph,
                         const std::string& name)
    : m_graph(graph),
      m_name(name)
{
}

PassBuilder& PassBuilder::reads(
    ResourceHandle resource)
{
    m_reads.push_back(resource);
    return *this;
}

PassBuilder& PassBuilder::writes(
    ResourceHandle resource)
{
    m_writes.push_back(resource);
    return *this;
}

void PassBuilder::execute(PassExecuteFn fn)
{
    m_graph.registerPass(
        m_name,
        m_reads,
        m_writes,
        std::move(fn));
}

}