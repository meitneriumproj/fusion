#include <fusion/graph/DependencyResolver.hpp>

namespace fusion {

void DependencyResolver::addPass(
    const PassNode& pass)
{
    m_passes.push_back(pass);
}

void DependencyResolver::resolve()
{
    // For now:
    // Execution order = insertion order.
    // Later: hazard detection + reordering.
}

}