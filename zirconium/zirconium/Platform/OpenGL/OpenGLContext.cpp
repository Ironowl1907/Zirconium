#include "zrpch.h"

#include "core.h"
#include "glad/glad.h"
#include "zirconium/Platform/OpenGL/OpenGLContext.h"

namespace zirconium {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_WindowHandle(windowHandle) {

    ZR_CORE_ASSERT(m_WindowHandle, "Window Handle is null!");
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ZR_CORE_ASSERT(status, "Failed to initialize GLAD!");

    ZR_CORE_INFO("OpenGL Renderer");
    ZR_CORE_INFO("    Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    ZR_CORE_INFO("    Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    ZR_CORE_INFO("    Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

#ifdef ZR_ENABLE_ASSERTS
    int versionMajor;
    int versionMinor;

    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

    ZR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 4),
                   "Zirconium requires at least OpenGL 4.4!");
#endif
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}
} // namespace zirconium
