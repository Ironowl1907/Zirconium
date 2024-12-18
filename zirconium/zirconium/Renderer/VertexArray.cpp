#include "zrpch.h"
#include "core.h"

#include "zirconium/Renderer/VertexArray.h"
#include "zirconium/Renderer/Renderer.h"

#include "zirconium/Platform/OpenGL/OpenGLVertexArray.h"

namespace zirconium {

VertexArray * VertexArray::Create() {
  switch (Renderer::GetAPI()) {
  case zirconium::RendererAPI::API::None:
    ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
    return nullptr;
  case zirconium::RendererAPI::API::OpenGL:
    return new OpenGLVertexArray();
  }
  ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
  return nullptr;
}
} // namespace zirconium
