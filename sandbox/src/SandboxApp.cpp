#include "imgui.h"
#include "zirconium.h"
#include <glm/ext/matrix_transform.hpp>

class ExampleLayer : public zirconium::Layer {
public:
    ExampleLayer()
        : Layer("Example")
        , m_OrthoCamera(-1.0f, 1.0f, -1.0f, 1.0f)
        , m_CameraPosition(0.0f, 0.0f, 0.0f) {

        std::shared_ptr<zirconium::VertexBuffer> VertexBuffer;
        std::shared_ptr<zirconium::IndexBuffer> IndexBuffer;

        m_VertexArray.reset(zirconium::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.2f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 0.2f, 1.0f, //
            0.0f,  0.5f,  0.0f, 0.0f, 0.3f, 0.9f, 1.0f, //
        };

        VertexBuffer.reset(zirconium::VertexBuffer::Create(vertices, sizeof(vertices)));
        VertexBuffer->Bind();

        zirconium::BufferLayout layout = {
            {zirconium::ShaderDataType::Float3, "a_Position"},
            {zirconium::ShaderDataType::Float4, "a_Color"},
        };
        VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(VertexBuffer);

        unsigned int indices[3] = {0, 1, 2};
        IndexBuffer.reset(
            zirconium::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(IndexBuffer);

        // Shaders
        std::string vertexShaderSrc = R"(
           #version 330 core
           layout(location = 0) in vec3 a_Position;
           layout(location = 1) in vec4 a_Color;

           out vec4 v_Color;

           uniform mat4 u_ProjectionViewMatrix;

           void main() {
               gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
               v_Color = a_Color;
           }
         )";

        std::string fragmentShaderSrc = R"(
           #version 330 core
           layout(location = 0) out vec4 color;
           in vec4 v_Color;

           void main() {
               color = v_Color;
           }
         )";

        m_Shader.reset(new zirconium::Shader(vertexShaderSrc, fragmentShaderSrc));
    }

    virtual void OnUpdate() override {
        if (zirconium::Input::IsKeyPressed(ZR_KEY_UP)) {
            m_CameraPosition.y -= m_CameraSpeed;
        } else if (zirconium::Input::IsKeyPressed(ZR_KEY_DOWN)) {
            m_CameraPosition.y += m_CameraSpeed;
        }
        if (zirconium::Input::IsKeyPressed(ZR_KEY_RIGHT)) {
            m_CameraPosition.x -= m_CameraSpeed;
        } else if (zirconium::Input::IsKeyPressed(ZR_KEY_LEFT)) {
            m_CameraPosition.x += m_CameraSpeed;
        }
        if (zirconium::Input::IsKeyPressed(ZR_KEY_Q)) {
          m_CameraRotation += m_CameraSpeed * 100;
        } else if (zirconium::Input::IsKeyPressed(ZR_KEY_E)) {
          m_CameraRotation -= m_CameraSpeed * 100;
        }

        zirconium::RenderCommand::SetClearColor(
            {0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        m_OrthoCamera.SetPosition(m_CameraPosition);
        m_OrthoCamera.SetRotation(m_CameraRotation);
        zirconium::Renderer::BeginScene(m_OrthoCamera);
        zirconium::Renderer::Submit(m_VertexArray, m_Shader);
        zirconium::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {}

    virtual void OnEvent(zirconium::Event& event) override {
        zirconium::EventDispatcher dispatcher(event);
    }

private:
    std::shared_ptr<zirconium::Shader> m_Shader;
    std::shared_ptr<zirconium::VertexArray> m_VertexArray;
    zirconium::Camera m_OrthoCamera;
    glm::vec3 m_CameraPosition;
    float m_CameraSpeed = 0.1f;
    float m_CameraRotation = 0.0f;
};

class SandBox : public zirconium::Application {
public:
    SandBox() {
        PushLayer(new ExampleLayer());
    }
    ~SandBox() {}
};

zirconium::Application* zirconium::CreateApplication() {
    return new SandBox();
}
