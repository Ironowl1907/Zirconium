#pragma once

#include "glm/glm.hpp"

namespace zirconium {

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Uniforms
    void SetUniformMatrix4fx(const std::string &name, const glm::mat4& mat);

private:
    uint32_t m_RendererID;
};
} // namespace zirconium
