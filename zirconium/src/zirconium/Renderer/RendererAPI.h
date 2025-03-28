#pragma once
#include "VertexArray.h"
#include "glm/glm.hpp"

namespace zirconium {

class RendererAPI {
public:
    enum class API {
        None = 0,
        OpenGL = 1,
    };

public:
    ~RendererAPI() = default;
    virtual void SetClearColor(glm::vec4& color) = 0;
    virtual void Init() = 0;
    virtual void Clear() = 0;

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
    virtual void SetLineWidth(const float& width) = 0;

    inline static API GetAPI() {
        return s_API;
    }

private:
    static API s_API;
};

}; // namespace zirconium
