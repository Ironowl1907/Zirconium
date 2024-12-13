#include "imgui.h"
#include "zirconium.h"
#include <glm/ext/matrix_transform.hpp>

// Tempo
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;

    Transform(glm::vec3 position, glm::vec3 rotation = {0.0f, 0.0f, 0.0f}, glm::vec3 scale = {1.0f, 1.0f, 1.0f})
        : Position(position)
        , Scale(scale)
        , Rotation(rotation) {}

    operator glm::mat4() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);

        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), Scale);

        return translationMatrix * rotationMatrix * scalingMatrix;
    }
};

class ExampleLayer : public zirconium::Layer {
public:
    ExampleLayer()
        : Layer("Example")
        , m_OrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f)
        , m_CameraPosition(0.0f, 0.0f, 0.0f)
        , m_PrimaryColor(0.8f, 0.1f, 0.2f, 1.0f)
        , m_SecondaryColor(0.1f, 0.1f, 0.7f, 1.0f)
        , m_Transformation({0.0f, 0.0f, 0.0f}) {

        std::shared_ptr<zirconium::VertexBuffer> VertexBuffer;
        std::shared_ptr<zirconium::IndexBuffer> IndexBuffer;

        m_VertexArray.reset(zirconium::VertexArray::Create());

        float vertices[4 * 7] = {
            -0.5f, 0.5f,  0.0f, 0.0f, 0.3f, 0.9f, 1.0f, //
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.2f, 1.0f, //
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.2f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.0f, 0.3f, 0.9f, 1.0f, //
        };

        VertexBuffer.reset(zirconium::VertexBuffer::Create(vertices, sizeof(vertices)));
        VertexBuffer->Bind();

        zirconium::BufferLayout layout = {
            {zirconium::ShaderDataType::Float3, "a_Position"},
            {zirconium::ShaderDataType::Float4, "a_Color"},
        };
        VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(VertexBuffer);

        unsigned int indices[] = {0, 1, 2, 2, 1, 3};
        IndexBuffer.reset(zirconium::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(IndexBuffer);

        // Shaders
        std::string vertexShaderSrc = R"(
           #version 330 core
           layout(location = 0) in vec3 a_Position;
           layout(location = 1) in vec4 a_Color;

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

        m_FlatColorShader.reset(zirconium::Shader::Create(vertexShaderSrc, fragmentShaderSrc));
    }

    virtual void OnUpdate(zirconium::TimeStep delta) override {
        if (zirconium::Input::IsKeyPressed(ZR_KEY_UP)) {
            m_CameraPosition.y -= m_CameraSpeed * delta;
        } else if (zirconium::Input::IsKeyPressed(ZR_KEY_DOWN)) {
            m_CameraPosition.y += m_CameraSpeed * delta;
        }
        if (zirconium::Input::IsKeyPressed(ZR_KEY_RIGHT)) {
            m_CameraPosition.x -= m_CameraSpeed * delta;
        } else if (zirconium::Input::IsKeyPressed(ZR_KEY_LEFT)) {
            m_CameraPosition.x += m_CameraSpeed * delta;
        }
        if (zirconium::Input::IsKeyPressed(ZR_KEY_Q)) {
            m_CameraRotation += m_CameraSpeed * 100 * delta;
        } else if (zirconium::Input::IsKeyPressed(ZR_KEY_E)) {
            m_CameraRotation -= m_CameraSpeed * 100 * delta;
        }

        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        m_OrthoCamera.SetRotation(m_CameraRotation);
        m_OrthoCamera.SetPosition(m_CameraPosition);
        zirconium::Renderer::BeginScene(m_OrthoCamera);

        m_FlatColorShader->Bind();

        // Bad code, I know...
        for (int i = 1; i <= 50; ++i) {
            for (int a = 1; a <= 50; ++a) {
                if ((i + a) % 2 == 0)
                    std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_FlatColorShader)
                        ->SetUniformFloat4("u_Color", m_PrimaryColor);
                else
                    std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_FlatColorShader)
                        ->SetUniformFloat4("u_Color", m_SecondaryColor);

                m_Transformation = Transform({0.11 * i, 0.11f * a, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.1f, 0.1, 0.1f});
                zirconium::Renderer::Submit(m_VertexArray, m_FlatColorShader, m_Transformation);
            }
        }
        zirconium::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override {
        m_OrthoCamera.CameraDebugUI();

        ImGui::Begin("Color Picker");

        // Use glm::value_ptr to pass glm::vec4 data as float pointers
        ImGui::ColorEdit4("Primary", glm::value_ptr(m_PrimaryColor));
        ImGui::ColorEdit4("Secondary", glm::value_ptr(m_SecondaryColor));

        ImGui::End();
    }

    virtual void OnEvent(zirconium::Event& event) override {
        zirconium::EventDispatcher dispatcher(event);
    }

private:
    std::shared_ptr<zirconium::Shader> m_FlatColorShader;
    std::shared_ptr<zirconium::VertexArray> m_VertexArray;
    zirconium::Camera m_OrthoCamera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation;
    float m_CameraSpeed = 1.0f;

    glm::vec4 m_PrimaryColor;
    glm::vec4 m_SecondaryColor;

    Transform m_Transformation;
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
