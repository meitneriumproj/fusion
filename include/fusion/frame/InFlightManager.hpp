#pragma once

#include <vector>
#include <memory>

namespace fusion {

class Device;
class FrameContext;

class InFlightManager {
public:
    explicit InFlightManager(Device& device, uint32_t framesInFlight = 3);
    ~InFlightManager();

    InFlightManager(const InFlightManager&) = delete;
    InFlightManager& operator=(const InFlightManager&) = delete;

    FrameContext& currentFrame();
    void advanceFrame();

    uint32_t frameIndex() const noexcept { return m_currentFrame; }
    uint32_t maxFrames() const noexcept { return static_cast<uint32_t>(m_frames.size()); }

private:
    void createFrames();

    Device& m_deviceRef;
    std::vector<std::unique_ptr<FrameContext>> m_frames;

    uint32_t m_currentFrame = 0;
    uint32_t m_framesInFlight;
};

}