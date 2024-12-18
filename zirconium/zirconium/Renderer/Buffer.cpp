#include "zrpch.h"

#include "zirconium/Platform/OpenGL/OpenGLBuffer.h"
#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/Renderer.h"
#include "core.h"

namespace zirconium {
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return new OpenGLVertexBuffer(vertices, size);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return new OpenGLIndexBuffer(indices, size);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

} // namespace zirconium
