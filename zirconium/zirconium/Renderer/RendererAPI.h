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
    virtual void SetClearColor(glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

    inline static API GetAPI() {
        return s_API;
    }

private:
    static API s_API;
};

}; // namespace zirconium