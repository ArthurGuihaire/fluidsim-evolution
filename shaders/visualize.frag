#version 330 core

uniform sampler2D colorTexture;

out vec4 fragColor;
in vec2 texCoords;

void main() {
    fragColor = vec4(abs(texture(colorTexture, texCoords).xy), 0.0f, 1.0f);
}
