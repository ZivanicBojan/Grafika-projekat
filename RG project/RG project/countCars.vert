#version 330 core

layout (location = 0) in vec3 aPos;  // Pozicija vertexa
layout (location = 1) in vec2 aTexCoord;  // Teksturne koordinate

out vec2 TexCoord;  // Izlazne teksturne koordinate

uniform float offsetX;  // Pomak na X osu

void main()
{
    // Pomaknite vertikale na X osovini
    gl_Position = vec4(aPos.x + offsetX, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;  // Teksturne koordinate se prenose u fragment shader
}
