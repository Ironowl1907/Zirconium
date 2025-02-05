#include "Buffer.h"
#include "zrpch.h"

#include "core.h"
#include "zirconium/Platform/OpenGL/OpenGLBuffer.h"
#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/Renderer.h"

namespace zirconium {

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexBuffer>(size);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLIndexBuffer>(indices, size);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

} // namespace zirconium
