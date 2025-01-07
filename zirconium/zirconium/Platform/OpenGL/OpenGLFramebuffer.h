#pragma once
#include "zirconium/Renderer/FrameBuffer.h"

namespace zirconium {

class OpenGLFrameBuffer : public FrameBuffer {
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& specs);
    virtual ~OpenGLFrameBuffer();

    void Invalidate();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual const FrameBufferSpecification& GetSpecification() const override {
        return m_Specification;
    }
    virtual uint32_t GetColorAttachmentRendererID() const override {
        return m_ColorAttachment;
    }

private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment, m_DepthAttachment;

    FrameBufferSpecification m_Specification;
};

} // namespace zirconium
