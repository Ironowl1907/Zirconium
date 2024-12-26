#include "Renderer2D.h"
#include "Texture.h"
#include "zrpch.h"

#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/RenderCommand.h"
#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/Texture.h"

#include <glm/ext/matrix_transform.hpp>

namespace zirconium {

struct Renderer2DStorage {
    Ref<VertexArray> QuadVertexArray;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;
};

static Renderer2DStorage* s_Data;

void Renderer2D::Init() {

    ZR_PROFILE_FUNCTION();

    s_Data = new Renderer2DStorage();

    s_Data->QuadVertexArray = zirconium::VertexArray::Create();

    float vertices[5 * 4] = {
        // Positions         // Texture Coordinates
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // Top-left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // Top-right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f  // Bottom-right
    };

    Ref<VertexBuffer> vertexBuffer = zirconium::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->Bind();

    zirconium::BufferLayout layout = {
        {zirconium::ShaderDataType::Float3, "a_Position"},
        {zirconium::ShaderDataType::Float2, "a_TextureCoords"},
    };
    vertexBuffer->SetLayout(layout);
    s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[] = {0, 1, 2, 2, 1, 3};
    Ref<IndexBuffer> indexBuffer = zirconium::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

    s_Data->WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data->WhiteTexture->SetData(&whiteTextureData, 1 * sizeof(whiteTextureData));

    s_Data->TextureShader = zirconium::Shader::Create("../sandbox/res/shaders/TextureShader.glsl");
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetInt("u_Texture", 0);
}
void Renderer2D::Shutdown() {
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthoCamera& camera) {
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetMatrix4f("u_ProjectionViewMatrix", camera.GetProjectionViewMatrix());
}
void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
                          const float& tilingFactor) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                          const float& tilingFactor) {

    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    // Here bind white texture
    s_Data->WhiteTexture->Bind();

    s_Data->TextureShader->SetMatrix4f("u_ModelMatrix",
                                       glm::translate(glm::mat4(1.0f), position) *
                                           glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture,
                             const float& tilingFactor) {
    DrawTexQuad({position.x, position.y, 0.0f}, size, texture);
}

void Renderer2D::DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture,
                             const float& tilingFactor) {
    texture->Bind();

    s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);

    s_Data->TextureShader->SetMatrix4f("u_ModelMatrix",
                                       glm::translate(glm::mat4(1.0f), position) *
                                           glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                 const glm::vec4& color, const float& tilingFactor) {

    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    // Here bind white texture
    s_Data->WhiteTexture->Bind();

    s_Data->TextureShader->SetMatrix4f(
        "u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                 const Ref<Texture> texture, const float& tilingFactor) {
    texture->Bind();

    s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);

    s_Data->TextureShader->SetMatrix4f(
        "u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)));

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}

void Renderer2D::DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                    const Ref<Texture> texture, const float& tilingFactor) {
    DrawRotatedTexQuad({position.x, position.y, 1.0f}, size, rotation, texture, tilingFactor);
}
void Renderer2D::DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                    const Ref<Texture> texture, const float& tilingFactor) {
    ZR_CORE_ASSERT(texture, "Texture is null!");
    texture->Bind();

    s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);

    s_Data->TextureShader->SetMatrix4f(
        "u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) *
                             glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)) *
                             glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)));

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
} // namespace zirconium
