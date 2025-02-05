#include "zrpch.h"
#include "core.h"

#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/Renderer.h"
#include "zirconium/Platform/OpenGL/OpenGLShader.h"

namespace zirconium {

Shader::~Shader() {}

Ref<Shader> Shader::Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc) {
    switch (Renderer::GetAPI()) {

    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;

    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string filePath) {
    switch (Renderer::GetAPI()) {

    case zirconium::RendererAPI::API::None:
        ZR_CORE_ASSERT(false, "'None' Render API is not supproted");
        return nullptr;

    case zirconium::RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLShader>(filePath);
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return nullptr;
}

void ShaderLibrary::Add(const Ref<Shader>& shader) {
    auto& name = shader->GetName();
    Add(name, shader);
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
    ZR_CORE_ASSERT(!exists(name), "A shader with this name already exists!");
    m_Shaders[name] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& filePath) {
    auto shader = Shader::Create(filePath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath) {
    auto shader = Shader::Create(filePath);
    Add(name, shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) {
    ZR_CORE_ASSERT(exists(name), "This shader doesn't exists!");
    return m_Shaders[name];
}

bool ShaderLibrary::exists(const std::string& shader) const {
    return m_Shaders.find(shader) != m_Shaders.end();
}

} // namespace zirconium
