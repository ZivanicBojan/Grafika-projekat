#version 330 core

layout(location = 0) in vec2 aPos; // Pozicija
layout(location = 1) in vec2 aTexCoord; // Koordinate teksture

out vec2 TexCoord; // Izlaz za fragment shader

uniform float offsetY;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y + offsetY, 0.0, 1.0); // Pomeraj pozicije sa offsetY
    TexCoord = aTexCoord; // Prosle?ivanje koordinata teksture u fragment shader
}
