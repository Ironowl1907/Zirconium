#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Texture.h"
#include "zrpch.h"

#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/RenderCommand.h"
#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/Texture.h"

#include <cstdint>
#include <glm/ext/matrix_transform.hpp>

namespace zirconium {

struct QuadVertex {
    glm::vec3 Postition;
    glm::vec4 Color;
    glm::vec2 TexCoord;
};

struct Renderer2DStorage {
    const uint32_t MaxQuads = 10000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices = MaxQuads * 6; // MUST BE DIVISIBLE FOR 6!!!

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;
};
static Renderer2DStorage s_Data;

void Renderer2D::Init() {

    ZR_PROFILE_FUNCTION();

    s_Data.QuadVertexArray = zirconium::VertexArray::Create();

    s_Data.QuadVertexBuffer = zirconium::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVertexBuffer->Bind();

    zirconium::BufferLayout layout = {
        {zirconium::ShaderDataType::Float3, "a_Position"},
        {zirconium::ShaderDataType::Float4, "a_Color"},
        {zirconium::ShaderDataType::Float2, "a_TextureCoords"},
    };
    s_Data.QuadVertexBuffer->SetLayout(layout);

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, 1 * sizeof(whiteTextureData));

    s_Data.TextureShader = zirconium::Shader::Create("../sandbox/res/shaders/TextureShader.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetInt("u_Texture", 0);
}
void Renderer2D::Shutdown() {}

void Renderer2D::BeginScene(const OrthoCamera& camera) {
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMatrix4f("u_ProjectionViewMatrix", camera.GetProjectionViewMatrix());

    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.QuadIndexCount = 0;
}
void Renderer2D::EndScene() {

    ZR_PROFILE_FUNCTION();

    uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

    Flush();
}

void Renderer2D::Flush() {

    ZR_PROFILE_FUNCTION();

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
                          const float& tilingFactor) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                          const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    s_Data.QuadVertexBufferPtr->Postition = position;
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Postition = {position.x + size.x, position.y, 0.0f};
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Postition = {position.x + size.x, position.y + size.y, 0.0f};
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Postition = {position.x, position.y + size.y, 0.0f};
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;
    // s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    // s_Data.WhiteTexture->Bind();

    // s_Data.TextureShader->SetMatrix4f("u_ModelMatrix",
    //                                   glm::translate(glm::mat4(1.0f), position) *
    //                                       glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

    // s_Data.QuadVertexArray->Bind();
    // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer2D::DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture,
                             const float& tilingFactor) {
    DrawTexQuad({position.x, position.y, 0.0f}, size, texture);
}

void Renderer2D::DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture,
                             const float& tilingFactor) {
    texture->Bind();

    s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

    s_Data.TextureShader->SetMatrix4f("u_ModelMatrix",
                                      glm::translate(glm::mat4(1.0f), position) *
                                          glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                 const glm::vec4& color, const float& tilingFactor) {

    s_Data.TextureShader->SetFloat4("u_Color", color);
    s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    // Here bind white texture
    s_Data.WhiteTexture->Bind();

    s_Data.TextureShader->SetMatrix4f(
        "u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}
void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                 const Ref<Texture>& texture, const float& tilingFactor) {
    texture->Bind();

    s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

    s_Data.TextureShader->SetMatrix4f(
        "u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)));

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}

void Renderer2D::DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                    const Ref<Texture>& texture, const float& tilingFactor) {
    DrawRotatedTexQuad({position.x, position.y, 1.0f}, size, rotation, texture, tilingFactor);
}
void Renderer2D::DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                    const Ref<Texture>& texture, const float& tilingFactor) {
    texture->Bind();

    s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);

    s_Data.TextureShader->SetMatrix4f(
        "u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)));

    s_Data.QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
}
} // namespace zirconium
