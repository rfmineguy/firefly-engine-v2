#version 330 core

out vec4 FragColor;
in vec4 frag_color;

void main() {
    FragColor = frag_color; //vec4(1.0f, 1.0f, 0.0f, 1.0f);
}