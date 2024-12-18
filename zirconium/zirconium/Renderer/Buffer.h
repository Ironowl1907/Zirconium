#pragma once
#include "core.h"
#include <cstdint>

namespace zirconium {

enum class ShaderDataType : uint8_t {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

// clang-format off
static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
      case zirconium::ShaderDataType::None: return 0;
        case ShaderDataType::Float:  return 4;
        case ShaderDataType::Float2: return 4 * 2; // 2 floats
        case ShaderDataType::Float3: return 4 * 3; // 3 floats
        case ShaderDataType::Float4: return 4 * 4; // 4 floats
        case ShaderDataType::Mat3:   return 4 * 3 * 3; // 3x3 matrix of floats
        case ShaderDataType::Mat4:   return 4 * 4 * 4; // 4x4 matrix of floats
        case ShaderDataType::Int:    return 4;
        case ShaderDataType::Int2:   return 4 * 2; // 2 integers
        case ShaderDataType::Int3:   return 4 * 3; // 3 integers
        case ShaderDataType::Int4:   return 4 * 4; // 4 integers
        case ShaderDataType::Bool:   return 1;     // 1 byte for a boolean
    ZR_CORE_ASSERT(false, "Unknown Shader Type!");
    }
return 0;
}
// clang-format on

struct BufferElement {
    std::string Name;
    ShaderDataType Type;
    bool Normalized;
    uint32_t Offset;
    uint32_t Size;

    BufferElement(const ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name)
        , Type(type)
        , Normalized(normalized)
        , Offset(0)
        , Size(ShaderDataTypeSize(type)) {}
};
// clang-format off
inline uint32_t GetComponentCount(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::None:  return 0;
        case ShaderDataType::Float:  return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Mat3:   return 3 * 3; // 3x3 matrix = 9 elements
        case ShaderDataType::Mat4:   return 4 * 4; // 4x4 matrix = 16 elements
        case ShaderDataType::Int:    return 1;
        case ShaderDataType::Int2:   return 2;
        case ShaderDataType::Int3:   return 3;
        case ShaderDataType::Int4:   return 4;
        case ShaderDataType::Bool:   return 1;
    ZR_CORE_ASSERT(false, "Unknown Shader Type!");
    }
}
// clang-format on

class BufferLayout {
public:
    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_BufferElements(elements) {
        CalculateOffsetAndStride();
    }

    inline uint32_t GetStride() const {
        return m_Stride;
    }
    inline const std::vector<BufferElement>& GetElements() const {
        return m_BufferElements;
    }

    std::vector<BufferElement>::iterator begin() {
        return m_BufferElements.begin();
    }
    std::vector<BufferElement>::iterator end() {
        return m_BufferElements.end();
    }

    std::vector<BufferElement>::const_iterator begin() const {
        return m_BufferElements.begin();
    }
    std::vector<BufferElement>::const_iterator end() const {
        return m_BufferElements.end();
    }

private:
    void CalculateOffsetAndStride() {
        m_Stride = 0;
        uint32_t offset = 0;
        for (auto& element : m_BufferElements) {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

private:
    std::vector<BufferElement> m_BufferElements;
    uint32_t m_Stride = 0;
};

class VertexBuffer {
public:
    virtual ~VertexBuffer() {}

    static VertexBuffer* Create(float* vertices, uint32_t size);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;
    virtual BufferLayout& GetLayout() = 0;
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() {}

    static IndexBuffer* Create(uint32_t* indices, uint32_t size);

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;
};
} // namespace zirconium
