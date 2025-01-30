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

    virtual uint32_t ReadPixel(uint32_t attachmentIndex, int x, int y) override;

    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
      ZR_CORE_ASSERT(index < m_ColorAttachments.size(), "");
        return m_ColorAttachments[index];
    }

    virtual void Resize(uint32_t width, uint32_t height) override;

private:
    uint32_t m_RendererID = 0;

    FrameBufferSpecification m_Specification;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecification;
    FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
};

} // namespace zirconium
