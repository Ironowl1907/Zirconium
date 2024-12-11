#pragma once
#include "RenderCommand.h"
#include "VertexArray.h"

#include "Camera.h"
#include "Shader.h"

namespace zirconium {

class Renderer {
public:
    static void BeginScene(const Camera& orthoCamera);
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertexArray,
                       const std::shared_ptr<Shader>& shader);

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
