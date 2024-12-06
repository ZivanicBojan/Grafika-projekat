#version 330 core

in vec2 TexCoord; // Koordinate teksture
out vec4 FragColor;

uniform sampler2D textureSampler; // Sampler za teksturu

void main()
{
    FragColor = texture(textureSampler, TexCoord); // Uzimanje boje sa teksture na osnovu koordinata
}
