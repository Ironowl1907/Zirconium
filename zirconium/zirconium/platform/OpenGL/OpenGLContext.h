#pragma once
#include "Renderer/GraphicsContext.h"

class GLFWwindow;

namespace zirconium {
class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(GLFWwindow* windowHandle);
    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

} // namespace zirconium
