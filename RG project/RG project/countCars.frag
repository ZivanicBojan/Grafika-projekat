#version 330 core

in vec2 TexCoord;  // Ulazne teksturne koordinate
out vec4 FragColor;  // Izlazna boja

uniform sampler2D textureSampler;  // Sampler za teksture

void main()
{
    FragColor = texture(textureSampler, TexCoord);  // U?itaj boju sa teksture
}
