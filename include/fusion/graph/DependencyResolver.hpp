#pragma once

#include <vector>
#include <fusion/graph/ResourceHandle.hpp>

namespace fusion {

struct PassNode {
    std::vector<ResourceHandle> reads;
    std::vector<ResourceHandle> writes;
};

class DependencyResolver {
public:
    void addPass(const PassNode& pass);
    void resolve();

    const std::vector<PassNode>& passes() const { return m_passes; }

private:
    std::vector<PassNode> m_passes;
};

}