#include "OpenGLRendererAPI.h"
#include "glad/glad.h"
#include "zrpch.h"

namespace zirconium {

void OpenGLRenderAPI::SetClearColor(glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLRenderAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

} // namespace zirconium
