#pragma once
#include "core.h"
#include "zirconium/Renderer/UniformBuffer.h"

namespace zirconium {

class OpenGLUniformBuffer : public UniformBuffer {
public:
    OpenGLUniformBuffer(const uint32_t size, const uint32_t binding);
    virtual ~OpenGLUniformBuffer();

    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0);

private:
private:
    uint32_t m_RendererID;
};

} // namespace zirconium
