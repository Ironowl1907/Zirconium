#include "zrpch.h"

#include "GLFW/glfw3.h"
#include "core.h"
#include "zirconium/Core/log/log.h"
#include "zirconium/Platform/linux/linux.h"
#include "zirconium/Platform/linux/linuxInput.h"
#include "zrpch.h"

#include "zirconium/Events/ApplicationEvent.h"
#include "zirconium/Events/KeyEvent.h"
#include "zirconium/Events/MouseEvent.h"

#include "zirconium/Platform/OpenGL/OpenGLContext.h"
#include <GL/gl.h>

namespace zirconium {
static bool s_GLFWInitialized = false;

static void GLFWErorrCallback(int error_code, const char* description) {
    ZR_CORE_ERROR("GLFW Error ({0}) with error code: {1}", description, error_code);
}

Window* Window::Create(const WindowProps& props) {
    return new LinuxWindow(props);
}

void LinuxWindow::init(const WindowProps& props) {
    m_windowData.Title = props.Title;
    m_windowData.Width = props.Width;
    m_windowData.Height = props.Height;

    ZR_CORE_INFO("Creating window: {0} {1} {2}", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        // TODO: Terminate system shutdown
        int success = glfwInit();
        ZR_CORE_ASSERT(success, "Could not initialize glfw!");
        s_GLFWInitialized = true;
    }
    m_window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
    m_Context = new OpenGLContext(m_window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_window, &m_windowData);
    SetVSync(true);

    // GLFW callbacks
    glfwSetErrorCallback(GLFWErorrCallback);
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(codepoint);
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
        case GLFW_RELEASE: {
            KeyReleasedEvent event(key);
            data.EventCallback(event);
            break;
        }
        case GLFW_PRESS: {
            KeyPressedEvent event(key, 0);
            data.EventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(key, 1);
            data.EventCallback(event);
            break;
        }
        }
    });
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {

        case GLFW_PRESS: {
            MousePressedButtonEvent event(button);
            data.EventCallback(event);
            break;
        }

        case GLFW_RELEASE: {
            MouseReleasedButtonEvent event(button);
            data.EventCallback(event);
            break;
        }
        }
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrollEvent event((float)xoffset, (float)yoffset);
        data.EventCallback(event);
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xpos, (float)ypos);
        data.EventCallback(event);
    });
}

LinuxWindow::LinuxWindow(const WindowProps& props) {
    init(props);
}
LinuxWindow::~LinuxWindow() {
    shutdown();
}

void LinuxWindow::onUpdate() {
    glfwPollEvents();
    m_Context->SwapBuffers();
}
void LinuxWindow::SetEventCallback(const EventCallbackFn& callback) {
    m_windowData.EventCallback = callback;
}

void LinuxWindow::SetVSync(bool enabled) {
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_windowData.VSync = enabled;
}
bool LinuxWindow::IsVSyncOn() const {
    return m_windowData.VSync;
}

void LinuxWindow::shutdown() {
    glfwDestroyWindow(m_window);
}

} // namespace zirconium
