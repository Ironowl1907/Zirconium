#pragma once
#include "zirconium/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

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
