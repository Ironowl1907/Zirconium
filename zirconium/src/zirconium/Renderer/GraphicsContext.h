#pragma once
namespace zirconium {
class GraphicsContext {
public:
    ~GraphicsContext() = default;
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
};

} // namespace zirconium
