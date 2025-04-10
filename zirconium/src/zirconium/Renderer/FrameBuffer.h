#pragma once

#include "core.h"
#include <cstdint>
#include <initializer_list>

namespace zirconium {

enum class FramebufferTextureFormat {

    None = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8,
};

struct FramebufferTextureSpecification {
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : TextureFormat(format) {}

    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    // TODO: Filtering/Wrap
};

struct FramebufferAttachmentSpecification {
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : Attachments(attachments) {}

    std::vector<FramebufferTextureSpecification> Attachments;
};

struct FrameBufferSpecification {
    uint32_t Width;
    uint32_t Height;
    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool SpawChainTarget = false;
};

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;
    virtual const FrameBufferSpecification& GetSpecification() const = 0;
    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual void ClearAttachment(uint32_t attachmentIndex, const int value) = 0;

    static Ref<FrameBuffer> Create(const FrameBufferSpecification& specs);
};

} // namespace zirconium
