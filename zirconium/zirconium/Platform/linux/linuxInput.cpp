#include "zirconium/Core/Application.h"
#include "zirconium/Core/input.h"
#include "zrpch.h"

#include "GLFW/glfw3.h"

namespace zirconium {

bool Input::IsKeyPressed(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

float Input::GetMouseX() {
    auto [x, y] = GetMousePosition();
    return x;
}

float Input::GetMouseY() {
    auto [x, y] = GetMousePosition();
    return y;
}

std::pair<float, float> Input::GetMousePosition() {
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
