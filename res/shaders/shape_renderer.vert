#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 u_color;
out vec4 frag_color;

void main() {
    frag_color = u_color;
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}