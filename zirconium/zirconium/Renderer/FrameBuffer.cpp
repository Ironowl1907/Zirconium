#include "zrpch.h"

#include "FrameBuffer.h"
#include "Renderer.h"

#include "zirconium/Platform/OpenGL/OpenGLFramebuffer.h"

namespace zirconium {

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specs) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLFrameBuffer>(specs);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}
} // namespace zirconium
