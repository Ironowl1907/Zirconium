#include "Renderer.h"
#include "zrpch.h"
#include <cwchar>

namespace zirconium {

// TODO: Finish BeginScene() and EndScene()
void Renderer::BeginScene() {}
void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace zirconium
