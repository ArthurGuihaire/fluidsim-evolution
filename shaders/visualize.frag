#version 330 core

uniform sampler2D colorTexture;

out vec4 fragColor;
in vec2 texCoords;

void main() {
    //vec4 texel = texture(colorTexture, texCoords);
    //fragColor = vec4(texel.x, texel.y, -texel.y, 1.0f);
    fragColor = texture(colorTexture, texCoords);
}
