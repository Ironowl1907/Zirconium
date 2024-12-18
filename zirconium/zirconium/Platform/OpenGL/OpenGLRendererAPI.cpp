#include "OpenGLRendererAPI.h"
#include "glad/glad.h"
#include "zrpch.h"
#include <GL/gl.h>

namespace zirconium {

void OpenGLRenderAPI::SetClearColor(glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLRenderAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::Init() {
    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
  glViewport(x, y, width, height);
}

void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

} // namespace zirconium