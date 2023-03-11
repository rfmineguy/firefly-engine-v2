#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec4 color;

out vec4 f_color;

void main() {
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    f_color = color;
}