#include "OpenGLVertexArray.h"
#include "glad/glad.h"
#include "zrpch.h"
#include <GL/gl.h>
#include <cstdint>

namespace zirconium {

static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::None:
        return 0;
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;

    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;

    case ShaderDataType::Bool:
        return GL_BOOL;

    case ShaderDataType::UInt:
        return GL_UNSIGNED_INT;
    }
    ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {

    ZR_PROFILE_FUNCTION();

    glCreateVertexArrays(1, &m_RenderID);
}

void OpenGLVertexArray::Bind() const {

    ZR_PROFILE_FUNCTION();

    glBindVertexArray(m_RenderID);
}
void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {

    ZR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout!");

    glBindVertexArray(m_RenderID);
    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();

    for (const auto& element : layout) {
        switch (element.Type) {
        case ShaderDataType::None:
            ZR_CORE_ASSERT(false, "");

        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index++, GetComponentCount(element.Type),
                                  ShaderDataTypeToOpenGlBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(), (const void*)(uintptr_t)element.Offset);
            break;
        }

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool:
        case ShaderDataType::UInt: {
            glEnableVertexAttribArray(index);
            glVertexAttribIPointer(index++, GetComponentCount(element.Type),
                                   ShaderDataTypeToOpenGlBaseType(element.Type), layout.GetStride(),
                                   (const void*)(uintptr_t)element.Offset);
            break;
        }
        }
    }
    m_VetexBuffers.push_back(vertexBuffer);
}
void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {

    ZR_PROFILE_FUNCTION();

    glBindVertexArray(m_RenderID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}

} // namespace zirconium
