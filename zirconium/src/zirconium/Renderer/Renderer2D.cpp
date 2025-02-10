#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Texture.h"
#include "zrpch.h"

#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/RenderCommand.h"
#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/Texture.h"
#include "zirconium/Renderer/UniformBuffer.h"

#include <cmath>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

#include <filesystem>

namespace zirconium {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
    int EntityID = -1;

    std::string ToString() const {
        std::ostringstream oss;
        oss << "QuadVertex: "
            << "  Position: (" << Position.x << ", " << Position.y << ", " << Position.z << ") "
            << "  Color: (" << Color.r << ", " << Color.g << ", " << Color.b << ", " << Color.a << ") "
            << "  TexCoord: (" << TexCoord.x << ", " << TexCoord.y << ") "
            << "  TexIndex: " << TexIndex << " "
            << "  TilingFactor: " << TilingFactor << " "
            << "  EntityID: " << EntityID;
        return oss.str();
    }
};

struct Renderer2DStorage {
    static const uint32_t MaxQuads = 10000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6; // MUST BE DIVISIBLE FOR 6!!!
    static const uint32_t MaxTextureSlots = 32;      // TODO: RenderCAPS

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // Texture slot 0 is for white texture

    glm::vec4 QuadVertexPositions[4];

    Renderer2D::Statistics Stats;

    struct CameraData {
        glm::mat4 ViewProjection;
    };
    CameraData CameraBuffer;
    Ref<UniformBuffer> CameraUniformBuffer;
};
static Renderer2DStorage s_Data;

static void SetVertexData(glm::mat4 transform, uint32_t textureIndex, glm::vec4 color, int EntityID,
                          float tilingFactor) {

    static glm::vec2 s_TextureCoords[4] = {
        {0.5f, 0.5f},
        {-0.5f, 0.5f},
        {0.5f, -0.5f},
        {-0.5f, -0.5f},
    };

    for (uint32_t i = 0; i < 4; i++) {
        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = s_TextureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr->EntityID = EntityID;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
}

void Renderer2D::Init() {

    ZR_PROFILE_FUNCTION();

    s_Data.QuadVertexArray = zirconium::VertexArray::Create();

    s_Data.QuadVertexBuffer = zirconium::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

    zirconium::BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},      {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float2, "a_TextureCoords"}, {ShaderDataType::Float, "a_TextureIndex"},
        {ShaderDataType::Float, "a_TilingFactor"},   {ShaderDataType::Int, "a_EntityID"},
    };

    s_Data.QuadVertexBuffer->SetLayout(layout);
    s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorage::CameraData), 0);

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, 1 * sizeof(whiteTextureData));

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
        samplers[i] = i;
    }

    std::filesystem::path path = "zirconium-Editor/res/shaders/TextureShader.glsl";
    std::filesystem::path fullPath = std::filesystem::absolute(path);
    s_Data.TextureShader = zirconium::Shader::Create(fullPath.c_str());
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}
void Renderer2D::Shutdown() {}

void Renderer2D::FlushAndReset() {
    EndScene();

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {

    ZR_PROFILE_FUNCTION();

    s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::BeginScene(const EditorCamera& camera) {

    ZR_PROFILE_FUNCTION();

    s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::EndScene() {

    ZR_PROFILE_FUNCTION();

    uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

    // for (QuadVertex* i = s_Data.QuadVertexBufferBase; i < s_Data.QuadVertexBufferPtr; i++)
    // ZR_CORE_WARN(i->ToString());

    Flush();
}

void Renderer2D::Flush() {
    ZR_PROFILE_FUNCTION();

    // Bind all the textures
    s_Data.TextureShader->Bind();
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
        s_Data.TextureSlots[i]->Bind(i);
    }

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    s_Data.Stats.DrawCalls++;
}

void Renderer2D::DrawTransformedQuad(const glm::mat4& transform, const glm::vec4& color) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    uint32_t tilingFactor = 1;
    SetVertexData(transform, 0, color, -1, tilingFactor);
}

void Renderer2D::DrawTransformedTexQuad(glm::mat4& transform, const Ref<Texture2D>& texture,
                                        const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 color(1.0f);

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if ((*s_Data.TextureSlots[i].get()).operator==(*texture.get())) {
            textureIndex = (float)i;
            break;
        }
    }

    if (!textureIndex) {
        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }
    SetVertexData(transform, textureIndex, color, -1, tilingFactor);
}

void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    uint32_t tilingFactor = 1;
    // Adding the +1 for now to allow zeroed ID entities
    SetVertexData(transform, 0, src, entityID + 1, tilingFactor);
}

Renderer2D::Statistics Renderer2D::GetStats() {
    return s_Data.Stats;
}
void Renderer2D::ResetStats() {
    // memset(&s_Data.Stats, 0, sizeof(Statistics));

    s_Data.Stats.DrawCalls = 0;
    s_Data.Stats.QuadCount = 0;
}

} // namespace zirconium
