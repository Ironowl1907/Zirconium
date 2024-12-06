#include "Buffer.h"
#include "Renderer.h"
#include "core.h"
#include "platform/OpenGL/OpenGLBuffer.h"
#include "zrpch.h"

namespace zirconium {
VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size) {
  switch (Renderer::GetRenderAPI()) {
  case zirconium::RenderAPI::None:
    ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
    return nullptr;
  case zirconium::RenderAPI::OpenGL:
    return new OpenGLVertexBuffer(vertices, size);
  }
  ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
  return nullptr;
}

IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t size) {
  switch (Renderer::GetRenderAPI()) {
  case zirconium::RenderAPI::None:
    ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
    return nullptr;
  case zirconium::RenderAPI::OpenGL:
    return new OpenGLIndexBuffer(indices, size);
  }
  ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
  return nullptr;
}

} // namespace zirconium
