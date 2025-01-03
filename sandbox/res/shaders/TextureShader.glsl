#type vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

out vec2 v_TexCoords;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

uniform mat4 u_ProjectionViewMatrix;

void main() {
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
    v_TexCoords = a_TexCoords;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
}

#type fragment
#version 460 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoords;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];
uniform float u_TilingFactor;

void main() {
    color = texture(u_Textures[int(v_TexIndex)], v_TexCoords * v_TilingFactor) * v_Color;
}
