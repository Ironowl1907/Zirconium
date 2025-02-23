#pragma once
#include "Camera.h"
#include "OrthographicCamera.h"
#include "SubTexture2D.h"
#include "zirconium/Renderer/EditorCamera.h"
#include "zirconium/Renderer/SubTexture2D.h"
#include "zirconium/Renderer/Texture.h"
#include "zirconium/scene/Components.h"

namespace zirconium {
class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const EditorCamera& camera);
    static void BeginScene(const Camera& camera, const glm::mat4& transform);
    static void EndScene();

    static void Flush();

    static void DrawTransformedQuad(const glm::mat4& transform, const glm::vec4& color);
    static void DrawTransformedTexQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, glm::vec4& tint,
                                       int entityID = -1, const float& tilingFactor = 1);

    static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

    static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, int entity,
                           const float& thickness = 1.0f, const float& fade = 0.00025f);
    static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);

    static void ClearVB();

private:
    static void FlushAndReset();
    static void BeginBatch();

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
