#pragma once

#include <string>
#include <functional>
#include <vector>

#include <fusion/graph/ResourceHandle.hpp>

namespace fusion {

class RenderGraph;
class PassContext;

using PassExecuteFn = std::function<void(PassContext&)>;

class PassBuilder {
public:
    PassBuilder(RenderGraph& graph, const std::string& name);

    PassBuilder& reads(ResourceHandle resource);
    PassBuilder& writes(ResourceHandle resource);

    void execute(PassExecuteFn fn);

private:
    RenderGraph& m_graph;
    std::string m_name;

    std::vector<ResourceHandle> m_reads;
    std::vector<ResourceHandle> m_writes;
};

}