#include <fusion/core/Surface.hpp>
#include <fusion/core/Instance.hpp>

namespace fusion {

Surface::Surface(Instance& instance,
                 VkSurfaceKHR surface)
    : m_instanceRef(instance),
      m_surface(surface)
{
}

Surface::~Surface() {
    if (m_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(
            m_instanceRef.vkInstance(),
            m_surface,
            nullptr);
        m_surface = VK_NULL_HANDLE;
    }
}

Surface::Surface(Surface&& other) noexcept
    : m_instanceRef(other.m_instanceRef),
      m_surface(other.m_surface)
{
    other.m_surface = VK_NULL_HANDLE;
}

Surface& Surface::operator=(Surface&& other) noexcept {
    if (this != &other) {
        if (m_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(
                m_instanceRef.vkInstance(),
                m_surface,
                nullptr);
        }

        m_surface = other.m_surface;
        other.m_surface = VK_NULL_HANDLE;
    }
    return *this;
}

}