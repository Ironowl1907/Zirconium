#include "zrpch.h"

#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"

namespace zirconium {

Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

void Renderer::BeginScene(const Camera& orthoCamera) {
    s_SceneData->ViewProjMatrix = orthoCamera.GetProjectionViewMatrix();
}
void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray,
                      const std::shared_ptr<Shader>& shader) {
    shader->Bind();
    shader->SetUniformMatrix4fx("u_ProjectionViewMatrix", s_SceneData->ViewProjMatrix);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace zirconium