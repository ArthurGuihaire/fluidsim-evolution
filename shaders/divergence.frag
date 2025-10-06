#version 330 core

uniform sampler2D velocityTexture;
uniform vec2 texelSize;

out vec2 fragColor;
in vec2 texCoords;

void main() {
    // Central differences scaled by grid spacing
    float du_dx = (texture(velocityTexture, texCoords + vec2(texelSize.x, 0.0f)).x -
                   texture(velocityTexture, texCoords - vec2(texelSize.x, 0.0f)).x) / (2.0f * texelSize.x);
    float dv_dy = (texture(velocityTexture, texCoords + vec2(0.0f, texelSize.y)).y -
                   texture(velocityTexture, texCoords - vec2(0.0f, texelSize.y)).y) / (2.0f * texelSize.y);
    float divergence = du_dx + dv_dy;
    fragColor = vec2(divergence, 0.0f);
}
