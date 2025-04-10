#pragma once

#include "zirconium/Renderer/RendererAPI.h"

namespace zirconium {
class OpenGLRenderAPI : public RendererAPI {
public:
    virtual void SetClearColor(glm::vec4& color) override;
    virtual void Clear() override;
    virtual void Init() override;

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
    virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
    virtual void SetLineWidth(const float& width) override;
};
} // namespace zirconium
