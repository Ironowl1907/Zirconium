#pragma once

#include <cstdint>
#include <string>

namespace zirconium {

class Shader {
public:
  Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
  ~Shader();

  void Bind() const;
  void Unbind() const;

private:
  uint32_t m_RendererID;
};
} // namespace zirconium
