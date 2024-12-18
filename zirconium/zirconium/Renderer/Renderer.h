#pragma once
#include "RenderCommand.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"

namespace zirconium {

class Renderer {
public:
    static void Init();

    static void BeginScene(const OrthoCamera& orthoCamera);
    static void EndScene();

    static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader,
                       const glm::mat4& transformation = glm::mat4(1.0f));

    inline static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

private:
    static Renderer s_RendererAPI;

    struct SceneData {
        glm::mat4 ViewProjMatrix;
    };
    static SceneData* s_SceneData;
};
} // namespace zirconium
