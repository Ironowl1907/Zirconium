#pragma once
#include "zirconium/Renderer/Camera.h"
#include "zirconium/Renderer/Texture.h"

namespace zirconium {
class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthoCamera& camera);
    static void EndScene();

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

    static void DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture);
    static void DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);
};

} // namespace zirconium
