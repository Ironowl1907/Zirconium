#pragma once
#include "Camera.h"
#include "SubTexture2D.h"
#include "OrthographicCamera.h"
#include "zirconium/Renderer/SubTexture2D.h"
#include "zirconium/Renderer/Texture.h"
#include "zirconium/Renderer/EditorCamera.h"

namespace zirconium {
class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthoCamera& camera); //TODO: REMOVE
    static void BeginScene(const EditorCamera& camera);
    static void BeginScene(const Camera& camera, const glm::mat4& transform);
    static void EndScene();

    static void Flush();

    static void DrawTransformedQuad(const glm::mat4& transform, const glm::vec4& color);
    static void DrawTransformedTexQuad(glm::mat4& transform, const Ref<Texture2D>& texture,
                                       const float& tilingFactor = 1);

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
                         const float& tilingFactor = 1);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                         const float& tilingFactor = 1);

    static void DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                            const float& tilingFactor = 1);
    static void DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                            const float& tilingFactor = 1);
    static void DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                            const float& tilingFactor = 1);
    static void DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                            const float& tilingFactor = 1);

    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                const glm::vec4& color, const float& tilingFactor = 1);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                glm::vec4 color, const float& tilingFactor = 1);

    static void DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                   const Ref<Texture2D>& texture, const float& tilingFactor = 1);
    static void DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                   const Ref<Texture2D>& texture, const float& tilingFactor = 1);
    static void DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                   const Ref<SubTexture2D>& subTexture, const float& tilingFactor = 1);
    static void DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                   const Ref<SubTexture2D>& subTexture, const float& tilingFactor = 1);

private:
    static void FlushAndReset();

    // Statistics

public:
    struct Statistics {
        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;

        uint32_t GetTotalVertexCount() {
            return QuadCount * 4;
        }
        uint32_t GetTotalIndexCount() {
            return QuadCount * 6;
        }
    };

    static Statistics GetStats();
    static void ResetStats();
};

} // namespace zirconium
