#pragma once
#include "Buffer.h"
#include <memory>
#include <vector>

namespace zirconium {

class VertexArray {
public:
    virtual ~VertexArray() {}

    static VertexArray* Create();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
    virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffers() = 0;
    virtual Ref<IndexBuffer>& GetIndexBuffer() = 0;
};
} // namespace zirconium
