#include "UniformBuffer.h"
#include "zrpch.h"

#include "zirconium/Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Renderer.h"

namespace zirconium {
Ref<UniformBuffer> UniformBuffer::Create(const uint32_t size, const uint32_t binding) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLUniformBuffer>(size, binding);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

} // namespace zirconium
