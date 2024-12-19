#include "VertexArray.h"
#include "core.h"
#include "zrpch.h"

#include "zirconium/Renderer/Renderer.h"
#include "zirconium/Renderer/VertexArray.h"

#include "zirconium/Platform/OpenGL/OpenGLVertexArray.h"
#include <memory>

namespace zirconium {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexArray>();
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}
} // namespace zirconium
