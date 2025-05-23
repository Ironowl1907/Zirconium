#pragma once

#include "core.h"

namespace zirconium {
class Input {
public:
    static bool IsKeyPressed(int keycode);

    static bool IsMouseButtonPressed(int button);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};
} // namespace zirconium
