#pragma once

#include "Renderer/Texture.h"
#include <cstdint>

namespace zirconium {
class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override {
        return m_Width;
    }
    virtual uint32_t GetHeight() const override {
        return m_Height;
    }

    virtual void Bind(const uint32_t slot) const override;

private:
    std::string m_Path;
    uint32_t m_Height, m_Width;
    uint32_t m_RendererID;
};
} // namespace zirconium
