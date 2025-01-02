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

    static void Flush();

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
                         const float& tilingFactor = 1);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                         const float& tilingFactor = 1);

    static void DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                            const float& tilingFactor = 1);
    static void DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                            const float& tilingFactor = 1);

    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                const glm::vec4& color, const float& tilingFactor = 1);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                glm::vec4 color, const float& tilingFactor = 1);

    static void DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                   const Ref<Texture2D>& texture, const float& tilingFactor = 1);
    static void DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                   const Ref<Texture2D>& texture, const float& tilingFactor = 1);
};

} // namespace zirconium
