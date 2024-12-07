#include "VertexArray.h"
#include "Renderer.h"
#include "core.h"
#include "zrpch.h"

#include "platform/OpenGL/OpenGLVertexArray.h"
#include <cmath>

namespace zirconium {

VertexArray * VertexArray::Create() {
  switch (Renderer::GetRenderAPI()) {
  case zirconium::RenderAPI::None:
    ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
    return nullptr;
  case zirconium::RenderAPI::OpenGL:
    return new OpenGLVertexArray();
  }
  ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
  return nullptr;
}
} // namespace zirconium
