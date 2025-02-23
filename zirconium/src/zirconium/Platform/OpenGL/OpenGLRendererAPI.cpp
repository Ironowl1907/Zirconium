#include "OpenGLRendererAPI.h"
#include "glad/glad.h"
#include "zrpch.h"
#include <GL/gl.h>
#include <cstdint>

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
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    vertexArray->Bind();
    uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
    vertexArray->Bind();
    ZR_CORE_WARN("Hey");
    glDrawArrays(GL_LINES, 0, vertexCount);
}

} // namespace zirconium
