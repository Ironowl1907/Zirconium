#pragma once
#include "core.h"

namespace zirconium {

class UniformBuffer {
public:
    virtual ~UniformBuffer() {}

    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
    static Ref<UniformBuffer> Create(const uint32_t size, const uint32_t binding);
};

} // namespace zirconium
