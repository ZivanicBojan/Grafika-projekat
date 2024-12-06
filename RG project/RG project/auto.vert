#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 fragTexCoord;

uniform float offsetX;
uniform float offsetY;

void main() {
    gl_Position = vec4(position.x + offsetX, position.y + offsetY, position.z, 1.0f);
    fragTexCoord = texCoord; // Prosledi teksturne koordinate
}
