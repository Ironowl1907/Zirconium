#include "Shader.h"
#include "Renderer.h"
#include "core.h"
#include "zrpch.h"

#include "platform/OpenGL/OpenGLShader.h"
#include <cmath>

namespace zirconium {

Shader::~Shader() {}

Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
    switch (Renderer::GetAPI()) {

    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;

    case zirconium::RendererAPI::API::OpenGL:
        return new OpenGLShader(vertexSrc, fragmentSrc);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

Shader* Shader::Create(const std::string& filePath) {
    switch (Renderer::GetAPI()) {

    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;

    case zirconium::RendererAPI::API::OpenGL:
        return new OpenGLShader(filePath);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}
} // namespace zirconium
