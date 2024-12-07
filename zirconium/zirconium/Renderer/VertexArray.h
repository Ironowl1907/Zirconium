#pragma once
#include "Buffer.h"
#include <memory>
#include <vector>

namespace zirconium {

class VertexArray {
public:
  virtual ~VertexArray() {}

  static VertexArray *Create();

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) = 0;
  virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) = 0;
  virtual std::vector<std::shared_ptr<VertexBuffer>> & GetVertexBuffers () = 0;
  virtual std::shared_ptr<IndexBuffer> & GetIndexBuffer () = 0;
};
} // namespace zirconium
