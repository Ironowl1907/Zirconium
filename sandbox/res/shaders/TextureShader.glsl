#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 u_ProjectionViewMatrix;
uniform vec4 u_Color;
uniform mat4 u_ModelMatrix;

void main() {
    gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
    v_TexCoords = a_TexCoords;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main() {
    color = texture(u_Texture, v_TexCoords);
    // color =  vec4(v_TexCoords, 0.0f,1.0f);
}
