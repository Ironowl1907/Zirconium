#pragma once

#include "Renderer/VertexArray.h"
#include <cstdint>
#include <memory>
#include <vector>

namespace zirconium {

class OpenGLVertexArray : public VertexArray {
public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray() {}

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
  virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

  virtual std::vector<std::shared_ptr<VertexBuffer>> & GetVertexBuffers () override {return m_VetexBuffers;}
  virtual std::shared_ptr<IndexBuffer> & GetIndexBuffer () override {return m_IndexBuffer;}

private:
  std::vector<std::shared_ptr<VertexBuffer>> m_VetexBuffers;
  std::shared_ptr<IndexBuffer> m_IndexBuffer;
  uint32_t m_RenderID;
};
} // namespace zirconium
