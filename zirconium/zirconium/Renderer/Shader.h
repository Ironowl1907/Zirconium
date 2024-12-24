#pragma once

#include "glm/glm.hpp"

namespace zirconium {

class Shader {
public:
    virtual ~Shader();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual const std::string& GetName() = 0;

    static Ref<Shader> Create(const std::string filePath);
    static Ref<Shader> Create(const std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);

    // Uniforms
    virtual void SetMatrix4f(const std::string& name, const glm::mat4& mat) = 0;
    virtual void SetFloat(const std::string& name, const float& n) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& vec) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& vec) = 0;
    virtual void SetInt(const std::string& name, const int& n) = 0;
};

class ShaderLibrary {
public:
    void Add(const Ref<Shader>& shader);
    void Add(const std::string& name, const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& filePath);
    Ref<Shader> Load(const std::string& name, const std::string& filePath);

    Ref<Shader> Get(const std::string& name);

private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    bool exists(const std::string& shader) const;
};
} // namespace zirconium
