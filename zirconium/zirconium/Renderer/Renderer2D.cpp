#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Texture.h"
#include "zrpch.h"

#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/RenderCommand.h"
#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/Texture.h"

#include <cmath>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace zirconium {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;

    std::string ToString() const {
        std::ostringstream oss;
        oss << "QuadVertex: "
            << "  Position: (" << Position.x << ", " << Position.y << ", " << Position.z << ") "
            << "  Color: (" << Color.r << ", " << Color.g << ", " << Color.b << ", " << Color.a << ") "
            << "  TexCoord: (" << TexCoord.x << ", " << TexCoord.y << ") "
            << "  TexIndex: " << TexIndex << " "
            << "  TilingFactor: " << TilingFactor;
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
};
static Renderer2DStorage s_Data;

void Renderer2D::Init() {

    ZR_PROFILE_FUNCTION();

    s_Data.QuadVertexArray = zirconium::VertexArray::Create();

    s_Data.QuadVertexBuffer = zirconium::VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVertexBuffer->Bind();

    zirconium::BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},      {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float2, "a_TextureCoords"}, {ShaderDataType::Float, "a_TextureIndex"},
        {ShaderDataType::Float, "a_TilingFactor"},
    };
    s_Data.QuadVertexBuffer->SetLayout(layout);

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

    s_Data.TextureShader = zirconium::Shader::Create("../sandbox/res/shaders/TextureShader.glsl");
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

void Renderer2D::BeginScene(const OrthoCamera& camera) {
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMatrix4f("u_ProjectionViewMatrix", camera.GetProjectionViewMatrix());

    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}
void Renderer2D::EndScene() {

    ZR_PROFILE_FUNCTION();

    uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

    Flush();
}

void Renderer2D::Flush() {
    ZR_PROFILE_FUNCTION();

    // // Log all QuadVertices
    // ZR_CORE_TRACE("------------------------------");
    // for (QuadVertex* vertex = s_Data.QuadVertexBufferBase; vertex < s_Data.QuadVertexBufferPtr; ++vertex) {
    //     ZR_CORE_TRACE(vertex->ToString());
    // }

    // Bind all the textures
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

    uint32_t texIndex = 0;
    uint32_t tilingFactor = 1;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawTransformedTexQuad(glm::mat4& transform, const Ref<Texture2D>& texture, const float& tilingFactor) {

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

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
                          const float& tilingFactor) {
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
                          const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    const float texIndex = 0.0f;

    glm::mat4 transform;
    {
        ZR_PROFILE_SCOPE("Transformation Matrix");
        transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
    }

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                             const float& tilingFactor) {
    DrawTexQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor);
}

void Renderer2D::DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                             const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 color(1.0f);

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

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

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                 const glm::vec4& color, const float& tilingFactor) {
    DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color, tilingFactor);
}
void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                 glm::vec4 color, const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    float textureIndex = 0.0f;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                    const Ref<Texture2D>& texture, const float& tilingFactor) {
    DrawRotatedTexQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor);
}

void Renderer2D::DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                    const Ref<Texture2D>& texture, const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 color(1.0f);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

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

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawTexQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                             const float& tilingFactor) {
    DrawTexQuad({position.x, position.y, 0.0f}, size, subTexture, tilingFactor);
}
void Renderer2D::DrawTexQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture,
                             const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 color(1.0f);
    const glm::vec2* textureCoords = subTexture->GetTexCoords();
    const Ref<Texture2D>& texture = subTexture->GetTexture();

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

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

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[0];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[1];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[2];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[3];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawRotatedTexQuad(const glm::vec2& position, const glm::vec2& size, const float& rotation,
                                    const Ref<SubTexture2D>& subTexture, const float& tilingFactor) {
    DrawRotatedTexQuad({position.x, position.y, 0.0f}, size, rotation, subTexture, tilingFactor);
}
void Renderer2D::DrawRotatedTexQuad(const glm::vec3& position, const glm::vec2& size, const float& rotation,
                                    const Ref<SubTexture2D>& subTexture, const float& tilingFactor) {

    ZR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
        FlushAndReset();

    constexpr glm::vec4 color(1.0f);
    const glm::vec2* textureCoords = subTexture->GetTexCoords();
    const Ref<Texture2D>& texture = subTexture->GetTexture();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

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

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[0];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[1];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[2];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
    s_Data.QuadVertexBufferPtr->Color = color;
    s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[3];
    s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

Renderer2D::Statistics Renderer2D::GetStats() {
    return s_Data.Stats;
}
void Renderer2D::ResetStats() {
    memset(&s_Data.Stats, 0, sizeof(Statistics));
}

} // namespace zirconium
