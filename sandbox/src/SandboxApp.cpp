#include "zirconium.h"
#include "zirconium/Core/entryPoint.h"
#include "Sandbox2D.h"

#include "imgui.h"
// Tempo
#include <filesystem>

class ExampleLayer : public zirconium::Layer {
public:
    ExampleLayer()
        : Layer("Example")
        , m_CameraControlloer(16.0f / 9.0f, true)
        , m_CameraPosition(0.0f, 0.0f, 0.0f)
        , m_CameraRotation(0.0f)
        , m_ShaderLib()
        , m_Transformation({0.0f, 0.0f, 0.0f}) {

        ZR_CORE_INFO("Working path:: {0}", std::filesystem::current_path().c_str());
        zirconium::Ref<zirconium::VertexBuffer> VertexBuffer;
        zirconium::Ref<zirconium::IndexBuffer> IndexBuffer;

        m_VertexArray = zirconium::VertexArray::Create();

        float vertices[4 * 5] = {
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // Top-left
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // Top-right
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
        };

        VertexBuffer = zirconium::VertexBuffer::Create(vertices, sizeof(vertices));
        VertexBuffer->Bind();

        zirconium::BufferLayout layout = {
            {zirconium::ShaderDataType::Float3, "a_Position"},
            {zirconium::ShaderDataType::Float2, "a_TexCoord"},
        };
        VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(VertexBuffer);

        unsigned int indices[] = {0, 1, 2, 2, 1, 3};
        IndexBuffer = zirconium::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(IndexBuffer);

        // Shaders
        std::string vertexShaderSrc = R"(
           #version 330 core
           layout(location = 0) in vec3 a_Position;

           out vec4 v_Color;

           uniform mat4 u_ProjectionViewMatrix;
           uniform vec4 u_Color;
           uniform mat4 u_ModelMatrix;

           void main() {
               gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
               v_Color = u_Color;
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

        m_FlatColorShader = zirconium::Shader::Create("TriangleShader", vertexShaderSrc, fragmentShaderSrc);

        m_TextureShader = zirconium::Shader::Create("../sandbox/res/shaders/TextureShader.glsl");

        // Textures
        m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");
        m_VLCTexture = zirconium::Texture2D::Create("../sandbox/res/textures/circle.png");

        std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_TextureShader)->SetUniformInt("u_Texture", 0);
    }

    virtual void OnUpdate(zirconium::TimeStep delta) override {

        m_CameraControlloer.OnUpdate(delta);

        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        zirconium::Renderer::BeginScene(m_CameraControlloer.GetCamera());

        m_Transformation = zirconium::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, glm::vec3(1.5f));

        m_Texture->Bind();
        zirconium::Renderer::Submit(m_VertexArray, m_TextureShader, m_Transformation);
        m_VLCTexture->Bind();
        zirconium::Renderer::Submit(m_VertexArray, m_TextureShader, m_Transformation);

        zirconium::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {
        m_CameraControlloer.CameraDebugUI();
    }

    virtual void OnEvent(zirconium::Event& event) override {
        m_CameraControlloer.OnEvent(event);
        zirconium::EventDispatcher dispatcher(event);
    }

private:
    zirconium::Ref<zirconium::ShaderLibrary> m_ShaderLib;
    zirconium::Ref<zirconium::Shader> m_FlatColorShader, m_TextureShader;

    zirconium::Ref<zirconium::VertexArray> m_VertexArray;

    zirconium::Ref<zirconium::Texture2D> m_Texture, m_VLCTexture;

    zirconium::OrthoCameraController m_CameraControlloer;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation;
    float m_CameraSpeed = 1.0f;

    zirconium::Transform m_Transformation;
};

class SandBox : public zirconium::Application {
public:
    SandBox() {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }
    ~SandBox() {}
};

zirconium::Application* zirconium::CreateApplication() {
    return new SandBox();
}
