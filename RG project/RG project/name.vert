#version 330 core
layout(location = 0) in vec2 aPos;  // Pozicija verteksa
layout(location = 1) in vec2 aTexCoord;  // Koordinate teksture

out vec2 TexCoord;  // Koordinate teksture za fragment shader

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;  // Prosledi koordinate teksture
}
