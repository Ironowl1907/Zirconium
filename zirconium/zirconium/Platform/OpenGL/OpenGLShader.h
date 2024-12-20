#pragma once

#include "zirconium/Renderer/Shader.h"
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <unistd.h>

namespace zirconium {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    OpenGLShader(const std::string& filePath);
    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const std::string& GetName() override {
        return m_Name;
    }

    void SetMatrix4f(const std::string& name, const glm::mat4& mat) override;
    void SetFloat3(const std::string& name, const glm::vec3& vec) override;
    void SetFloat4(const std::string& name, const glm::vec4& vec) override;

    // Uniforms
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& mat);
    void SetUniformFloat4(const std::string& name, const glm::vec4 vec);
    void SetUniformFloat3(const std::string& name, const glm::vec3 vec);
    void SetUniformInt(const std::string& name, const int n);

private:
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs);
    std::string ReadFile(const std::string& filePath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& shaderSrc);

private:
    uint32_t m_RendererID;
    std::string m_Name;
};
} // namespace zirconium
