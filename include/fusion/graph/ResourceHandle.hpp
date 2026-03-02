#pragma once

#include <cstdint>

namespace fusion {

enum class ResourceType {
    Texture,
    Buffer
};

class ResourceHandle {
public:
    ResourceHandle() = default;
    ResourceHandle(uint32_t id, ResourceType type)
        : m_id(id), m_type(type) {}

    uint32_t id() const noexcept { return m_id; }
    ResourceType type() const noexcept { return m_type; }

    bool valid() const noexcept { return m_id != UINT32_MAX; }

    static ResourceHandle invalid() {
        return ResourceHandle(UINT32_MAX, ResourceType::Texture);
    }

private:
    uint32_t m_id = UINT32_MAX;
    ResourceType m_type = ResourceType::Texture;
};

}