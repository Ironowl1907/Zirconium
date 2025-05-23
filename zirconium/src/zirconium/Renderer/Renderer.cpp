#include "zirconium/Renderer/RenderCommand.h"
#include "zrpch.h"

#include "zirconium/Platform/OpenGL/OpenGLShader.h"
#include "zirconium/Renderer/Camera.h"
#include "zirconium/Renderer/Renderer.h"
#include "zirconium/Renderer/Renderer2D.h"

namespace zirconium {

Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

void Renderer::Init() {

  ZR_PROFILE_FUNCTION();

    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::BeginScene(const OrthoCamera& orthoCamera) {
    s_SceneData->ViewProjMatrix = orthoCamera.GetProjectionViewMatrix();
}
void Renderer::EndScene() {}

void Renderer::OnWindowResize(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transformation) {
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatrix4f("u_ProjectionViewMatrix",
                                                                        s_SceneData->ViewProjMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMatrix4f("u_ModelMatrix", transformation);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace zirconium
