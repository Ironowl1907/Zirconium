#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Texture.h"
#include "zrpch.h"

#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/RenderCommand.h"
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
};

struct CircleVertex {
    glm::vec3 WorldPosition;
    glm::vec3 LocalPosition;
    glm::vec4 Color;
    float Thickness;
    float Fade;
    int EntityID = -1;
};

struct LineVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    int EntityID = -1;
};

[[maybe_unused]] static std::string QuadVertexToString(const QuadVertex& vert) {
    std::ostringstream oss;
    oss << "QuadVertex: "
        << "  Position: (" << vert.Position.x << ", " << vert.Position.y << ", " << vert.Position.z << ") "
        << "  Color: (" << vert.Color.r << ", " << vert.Color.g << ", " << vert.Color.b << ", " << vert.Color.a << ") "
        << "  TexCoord: (" << vert.TexCoord.x << ", " << vert.TexCoord.y << ") "
        << "  TexIndex: " << vert.TexIndex << " "
        << "  TilingFactor: " << vert.TilingFactor << " "
        << "  EntityID: " << vert.EntityID;
    return oss.str();
}

[[maybe_unused]] static std::string LineVertexToString(const LineVertex& vert) {
    std::ostringstream oss;
    oss << "LineVert: "
        << "  Position: (" << vert.Position.x << ", " << vert.Position.y << ", " << vert.Position.z << ") "
        << "  Color: (" << vert.Color.r << ", " << vert.Color.g << ", " << vert.Color.b << ", " << vert.Color.a << ") "
        << "  EntityID: " << vert.EntityID;
    return oss.str();
}

struct Renderer2DStorage {
    static const uint32_t MaxQuads = 10000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6; // MUST BE DIVISIBLE FOR 6!!!
    static const uint32_t MaxTextureSlots = 32;      // TODO: RenderCAPS

    // Quads
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer = nullptr;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    // Circles
    uint32_t CircleIndexCount = 0;
    CircleVertex* CircleVertexBufferBase = nullptr;
    CircleVertex* CircleVertexBufferPtr = nullptr;

    Ref<VertexArray> CircleVertexArray;
    Ref<VertexBuffer> CircleVertexBuffer = nullptr;
    Ref<Shader> CircleShader;

    // Lines
    uint32_t LineVertexCount = 0;
    LineVertex* LineVertexBufferBase = nullptr;
    LineVertex* LineVertexBufferPtr = nullptr;

    Ref<VertexArray> LineVertexArray;
    Ref<VertexBuffer> LineVertexBuffer = nullptr;
    Ref<Shader> LineShader;

    float LineWidth = 2.0f;

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
        {0.0f, 0.0f}, // Bottom-left corner
        {1.0f, 0.0f}, // Bottom-right corner
        {1.0f, 1.0f}, // Top-right corner
        {0.0f, 1.0f}, // Top-left corner
    };

    for (uint32_t i = 0; i < 4; i++) {
        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = s_TextureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr->EntityID = EntityID + 1;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
}

void Renderer2D::ClearVB() {
    s_Data.QuadVertexBuffer->Clear(s_Data.MaxVertices * sizeof(QuadVertex));
}

void Renderer2D::Init() {

    ZR_PROFILE_FUNCTION();

    // Quads
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

    // Circles
    s_Data.CircleVertexArray = zirconium::VertexArray::Create();

    s_Data.CircleVertexBuffer = zirconium::VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
    zirconium::BufferLayout circleLayout = {
        {ShaderDataType::Float3, "a_WorldPosition"}, {ShaderDataType::Float3, "a_LocalPosition"},
        {ShaderDataType::Float4, "a_Color"},         {ShaderDataType::Float, "a_Thickness"},
        {ShaderDataType::Float, "a_Fade"},           {ShaderDataType::Int, "a_EntityID"},
    };
    s_Data.CircleVertexBuffer->SetLayout(circleLayout);

    s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
    s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
    s_Data.CircleVertexArray->SetIndexBuffer(quadIB);

    // Lines
    s_Data.LineVertexArray = zirconium::VertexArray::Create();

    s_Data.LineVertexBuffer = zirconium::VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
    zirconium::BufferLayout lineLayout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Int, "a_EntityID"},
    };
    s_Data.LineVertexBuffer->SetLayout(lineLayout);

    s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];
    s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, 1 * sizeof(whiteTextureData));

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
        samplers[i] = i;
    }

    // Quad Shader
    {
        std::filesystem::path path = "zirconium-Editor/res/shaders/TextureShader.glsl";
        std::filesystem::path fullPath = std::filesystem::absolute(path);
        s_Data.TextureShader = zirconium::Shader::Create(fullPath.c_str());
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
    }

    // Circle Shader
    {
        std::filesystem::path path = "zirconium-Editor/res/shaders/CircleShader.glsl";
        std::filesystem::path fullPath = std::filesystem::absolute(path);
        s_Data.CircleShader = zirconium::Shader::Create(fullPath.c_str());
    }

    // Line Shader
    {
        std::filesystem::path path = "zirconium-Editor/res/shaders/LineShader.glsl";
        std::filesystem::path fullPath = std::filesystem::absolute(path);
        s_Data.LineShader = zirconium::Shader::Create(fullPath.c_str());
    }

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}
void Renderer2D::Shutdown() {}

void Renderer2D::FlushAndReset() {
    EndScene();

    BeginBatch();

    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::BeginBatch() {

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.CircleIndexCount = 0;
    s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

    s_Data.LineVertexCount = 0;
    s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {

    ZR_PROFILE_FUNCTION();

    s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

    BeginBatch();
}

void Renderer2D::BeginScene(const EditorCamera& camera) {

    ZR_PROFILE_FUNCTION();

    s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
    s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

    BeginBatch();
}

void Renderer2D::EndScene() {

    ZR_PROFILE_FUNCTION();

    Flush();
}

void Renderer2D::Flush() {
    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount) {

        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        // Bind all the textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
            s_Data.TextureSlots[i]->Bind(i);
        }

        s_Data.TextureShader->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }
    if (s_Data.CircleIndexCount) {

        uint32_t dataSize = (uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase;
        s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

        s_Data.CircleShader->Bind();
        RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    if (s_Data.LineVertexCount) {

        uint32_t dataSize = (uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase;
        ZR_ASSERT(dataSize, "DataSize can't be zero!");
        s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

        s_Data.LineShader->Bind();
        RenderCommand::SetLineWidth(s_Data.LineWidth);
        RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
        s_Data.Stats.DrawCalls++;
    }
}

void Renderer2D::DrawTransformedQuad(const glm::mat4& transform, const glm::vec4& color) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    uint32_t tilingFactor = 1;
    SetVertexData(transform, 0, color, -1, tilingFactor);
}

void Renderer2D::DrawTransformedTexQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, glm::vec4& tint,
                                        int entityID, const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

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
    SetVertexData(transform, textureIndex, tint, entityID, tilingFactor);
}

void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    // Adding the +1 for now to allow zeroed ID entities
    // SetVertexData(transform, 0, src, entityID + 1, tilingFactor);
    DrawTransformedTexQuad(transform, src.Texture, src.Color, entityID, src.TilingFactor);

    // For Wireframe-ish mode
    // DrawRect(transform, src.Color, entityID);
}

void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, int entity, const float& thickness,
                            const float& fade) {

    ZR_PROFILE_FUNCTION();

    // TODO : Inplement for circles
    // if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
    //     FlushAndReset();

    for (uint32_t i = 0; i < 4; i++) {
        s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
        s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
        s_Data.CircleVertexBufferPtr->Color = color;
        s_Data.CircleVertexBufferPtr->Thickness = thickness;
        s_Data.CircleVertexBufferPtr->Fade = fade;
        s_Data.CircleVertexBufferPtr->EntityID = entity + 1;
        s_Data.CircleVertexBufferPtr++;
    }

    s_Data.CircleIndexCount += 6;
    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID) {
    s_Data.LineVertexBufferPtr->Position = p0;
    s_Data.LineVertexBufferPtr->Color = color;
    s_Data.LineVertexBufferPtr->EntityID = entityID;
    s_Data.LineVertexBufferPtr++;

    s_Data.LineVertexBufferPtr->Position = p1;
    s_Data.LineVertexBufferPtr->Color = color;
    s_Data.LineVertexBufferPtr->EntityID = entityID;
    s_Data.LineVertexBufferPtr++;

    s_Data.LineVertexCount += 2;
}

float Renderer2D::GetLineWidth() {
    return s_Data.LineWidth;
}
void Renderer2D::SetLineWidth(float width) {
    RenderCommand::SetLineWidth(width);
}

void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                          const int& entityID) {
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

    DrawLine(p0, p1, color, entityID);
    DrawLine(p1, p2, color, entityID);
    DrawLine(p2, p3, color, entityID);
    DrawLine(p3, p0, color, entityID);
}
void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, const int& entityID) {
    glm::vec3 vertex[4];
    for (int i = 0; i < 4; i++)
        vertex[i] = transform * s_Data.QuadVertexPositions[i];

    DrawLine(vertex[0], vertex[1], color);
    DrawLine(vertex[1], vertex[2], color);
    DrawLine(vertex[2], vertex[3], color);
    DrawLine(vertex[3], vertex[0], color);
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
