// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
};

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoords;
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
    Output.Color = a_Color;
    Output.TexCoords = a_TexCoords;
    Output.TexIndex = a_TexIndex;
    Output.TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoords;
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor = Input.Color;
    color = texture(u_Textures[int(Input.TexIndex)], Input.TexCoords * Input.TilingFactor) * Input.Color;

    color2 = v_EntityID;
    // if (v_EntityID == 0) color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    // if (v_EntityID == 1) color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    // if (v_EntityID == 2) color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    // if (v_EntityID == 3) color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    // color2 = v_EntityID;
}
