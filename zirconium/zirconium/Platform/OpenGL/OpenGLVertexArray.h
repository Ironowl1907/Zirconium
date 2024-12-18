#pragma once

#include "zirconium/Renderer/VertexArray.h"

namespace zirconium {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() {}

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

    virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override {
        return m_VetexBuffers;
    }
    virtual Ref<IndexBuffer>& GetIndexBuffer() override {
        return m_IndexBuffer;
    }

private:
    std::vector<Ref<VertexBuffer>> m_VetexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
    uint32_t m_RenderID;
};
} // namespace zirconium
