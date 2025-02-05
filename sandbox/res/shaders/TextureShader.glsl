#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjection;
};

struct VertexOutput {
    vec4 Color;
    vec2 TexCoords; // Corrected variable name
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) out VertexOutput Output; // Output structure
layout(location = 1) out flat int v_EntityID; // Changed location to avoid conflict

void main() {
    Output.Color = a_Color;
    Output.TexCoords = a_TexCoords; // Corrected variable name
    Output.TexIndex = a_TexIndex;
    Output.TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;
layout(location = 1) out int enttID;

struct VertexOutput {
    vec4 Color;
    vec2 TexCoords; // Ensure this matches the vertex shader output
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input; // Ensure input matches output from vertex shader
layout(location = 1) in flat int v_EntityID; // Changed location to match output from vertex shader

layout(binding = 0) uniform sampler2D u_Textures[32];

void main() {
    color = texture(u_Textures[int(Input.TexIndex)], Input.TexCoords * Input.TilingFactor) * Input.Color; // Corrected variable names
    enttID = v_EntityID;
}
