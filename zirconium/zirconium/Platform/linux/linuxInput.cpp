#include "zirconium/Platform/linux/linuxInput.h"
#include "zirconium/Core/Application.h"
#include "zrpch.h"

#include "GLFW/glfw3.h"

namespace zirconium {

Input* Input::s_Instance = new LinuxInput();

bool LinuxInput::IsKeyPressedImpl(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool LinuxInput::IsMouseButtonPressedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

float LinuxInput::GetMouseXImpl() {
    auto [x, y] = GetMousePosImpl();
    return x;
}

float LinuxInput::GetMouseYImpl() {
    auto [x, y] = GetMousePosImpl();
    return y;
}

std::pair<float, float> LinuxInput::GetMousePosImpl() {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    if (!window) {
        ZR_ERROR("Failed to retrieve native window pointer.");
        return {0.0f, 0.0f};
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float adjustedY = static_cast<float>(height) - static_cast<float>(ypos);

    return {static_cast<float>(xpos), adjustedY};
}

} // namespace zirconium
