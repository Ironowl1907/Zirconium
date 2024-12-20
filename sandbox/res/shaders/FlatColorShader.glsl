#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;

out vec4 v_Color;

uniform mat4 u_ProjectionViewMatrix;

void main() {
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main() {
    color = u_Color;
}
