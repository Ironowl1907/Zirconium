#pragma once

#include "Renderer/RendererAPI.h"

namespace zirconium {
class OpenGLRenderAPI : public RendererAPI {
public:
    virtual void SetClearColor(glm::vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};

} // namespace zirconium
