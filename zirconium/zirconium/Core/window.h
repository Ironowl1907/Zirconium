#pragma once
#include "zirconium/Events/Event.h"
#include <cstdint>

namespace zirconium {

struct WindowProps {
    WindowProps(const std::string& title = "Zirconium Engine", uint32_t width = 1280, uint32_t height = 720)
        : Title(title)
        , Width(width)
        , Height(height) {}

    std::string Title;
    uint32_t Width;
    uint32_t Height;
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void onUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

    virtual void SetVSync(bool enbabled) = 0;
    virtual bool IsVSyncOn() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
};

} // namespace zirconium
