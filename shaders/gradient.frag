#version 330 core

in vec2 texCoords;
uniform sampler2D divPresTexture; // We only care about pressure (G channel)
uniform sampler2D velocityTexture;
uniform vec2 texelSize;
out vec2 newVelocity;

void main() {
    // Central differences with proper scaling
    float dpdx = (texture(divPresTexture, texCoords + vec2(texelSize.x, 0.0f)).g -
                  texture(divPresTexture, texCoords - vec2(texelSize.x, 0.0f)).g) / (2.0f * texelSize.x);
    float dpdy = (texture(divPresTexture, texCoords + vec2(0.0f, texelSize.y)).g -
                  texture(divPresTexture, texCoords - vec2(0.0f, texelSize.y)).g) / (2.0f * texelSize.y);
    vec2 gradP = vec2(dpdx, dpdy);
    newVelocity = texture(velocityTexture, texCoords).xy - gradP;
}
