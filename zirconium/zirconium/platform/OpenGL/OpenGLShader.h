#pragma once

#include "Renderer/Shader.h"
#include "glm/glm.hpp"

namespace zirconium {

class OpenGLShader : public Shader {
public:
  OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    // Uniforms
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& mat);
    void SetUniformFloat4(const std::string& name, const glm::vec4 vec);
    void SetUniformInt(const std::string& name, const int n);

private:
    uint32_t m_RendererID;
};
} // namespace zirconium
