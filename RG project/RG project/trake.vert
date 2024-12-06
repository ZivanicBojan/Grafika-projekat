#version 330 core

layout(location = 0) in vec2 position;

uniform float offsetX;

void main() {
    vec2 pos = position;
    pos.x += offsetX; // Dodaj horizontalni pomak
    gl_Position = vec4(pos, 0.0, 1.0);
}
