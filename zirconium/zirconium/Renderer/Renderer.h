#pragma once

namespace zirconium {
enum class RenderAPI {
    None = 0,
    OpenGL = 1,
};

class Renderer {
public:
    static inline RenderAPI GetRenderAPI() {
        return s_RenderAPI;
    }
    static inline void SetRenderAPI(RenderAPI api) {
        s_RenderAPI = api;
    }

private:
    static RenderAPI s_RenderAPI;
};
} // namespace zirconium
