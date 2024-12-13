#pragma once

namespace zirconium {

class Shader {
public:
    virtual ~Shader();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

};
} // namespace zirconium
