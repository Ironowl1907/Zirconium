#include "Texture.h"
#include "core.h"
#include "zrpch.h"

#include "zirconium/Platform/OpenGL/OpenGLTexture.h"
#include "zirconium/Renderer/Renderer.h"
#include "zirconium/Renderer/Texture.h"

namespace zirconium {
Ref<Texture2D> Texture2D::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLTexture2D>(path);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const uint32_t& width, const uint32_t& height) {
    switch (Renderer::GetAPI()) {
    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;
    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLTexture2D>(width, height);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

void Texture2DLibrary::Init() {
    m_Instance = new Texture2DLibrary();
}

Texture2DLibrary::Texture2DLibrary() {}
Texture2DLibrary::~Texture2DLibrary() {
    delete m_Instance;
}

Ref<Texture2D> Texture2DLibrary::Add(const std::string& path) {
  return {};
}
Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filePath) {
    return {};
}

Texture2DLibrary* Texture2DLibrary::m_Instance = nullptr;
} // namespace zirconium
