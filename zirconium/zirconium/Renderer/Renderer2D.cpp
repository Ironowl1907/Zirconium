#include "zrpch.h"

#include "Buffer.h"
#include "Renderer2D.h"

#include "zirconium/Platform/OpenGL/OpenGLShader.h"
#include "zirconium/Renderer/RenderCommand.h"
#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/Renderer/Shader.h"

namespace zirconium {

struct Renderer2DStorage {
    Ref<VertexArray> QuadVertexArray;
    Ref<Shader> QuadShader;
};

static Renderer2DStorage* s_Data;

void Renderer2D::Init() {
    s_Data = new Renderer2DStorage();

    s_Data->QuadVertexArray = zirconium::VertexArray::Create();

    float vertices[3 * 4] = {
        -0.5f, 0.5f,  0.0f, //
        -0.5f, -0.5f, 0.0f, //
        0.5f,  0.5f,  0.0f, //
        0.5f,  -0.5f, 0.0f, //
    };

    Ref<VertexBuffer> vertexBuffer = zirconium::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->Bind();

    zirconium::BufferLayout layout = {
        {zirconium::ShaderDataType::Float3, "a_Position"},
    };
    vertexBuffer->SetLayout(layout);
    s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[] = {0, 1, 2, 2, 1, 3};
    Ref<IndexBuffer> indexBuffer = zirconium::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

    s_Data->QuadShader = zirconium::Shader::Create("../sandbox/res/shaders/FlatColorShader.glsl");
}
void Renderer2D::Shutdown() {
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthoCamera& camera) {
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)
        ->SetUniformMatrix4f("u_ProjectionViewMatrix", camera.GetProjectionViewMatrix());
}
void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
  ZR_CORE_TRACE("Drawing quad");
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->SetUniformFloat4("u_Color", color);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
}
} // namespace zirconium
