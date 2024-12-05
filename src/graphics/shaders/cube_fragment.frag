#version 330 core
out vec4 fragmentColor;
in vec2 texCoords;

uniform sampler2D texture1;

void main() {
    fragmentColor = texture(texture1, texCoords);
}
