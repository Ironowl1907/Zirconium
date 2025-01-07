#pragma once

#include "core.h"

namespace zirconium {

struct FrameBufferSpecification {
    uint32_t Width;
    uint32_t Height;
    uint32_t Samples = 1;

    bool SpawChainTarget = false;
};

class FrameBuffer {
public:
    virtual const FrameBufferSpecification& GetSpecification() const = 0;
    virtual uint32_t GetColorAttachmentRendererID() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    static Ref<FrameBuffer> Create(const FrameBufferSpecification& specs);
};

} // namespace zirconium
