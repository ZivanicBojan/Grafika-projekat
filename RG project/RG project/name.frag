#version 330 core
out vec4 FragColor;

in vec2 TexCoord;  // Koordinate teksture

uniform sampler2D ourTexture;  // Tekstura

void main() {
    FragColor = texture(ourTexture, TexCoord);  
}
