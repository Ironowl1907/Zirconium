#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoords;

out vec2 v_TexCoords;
out vec4 v_Color;

uniform mat4 u_ProjectionViewMatrix;

void main() {
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
    v_TexCoords = a_TexCoords;
    v_Color = a_Color;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoords;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TilingFactor;

void main() {
    // vec4 texColor = texture(u_Texture, v_TexCoords * u_TilingFactor);
    // color = texColor * u_Color;
    color = v_Color;
}
