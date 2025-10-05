#version 330 core

uniform sampler2D velocityTexture;
uniform vec2 texelSize;

out vec2 fragColor;
in vec2 texCoords;

void main() {
    float divergence = -texture(velocityTexture, texCoords - vec2(texelSize.x, 0.0f)).x + 
                        texture(velocityTexture, texCoords + vec2(texelSize.x, 0.0f)).x - 
                        texture(velocityTexture, texCoords - vec2(0.0f, texelSize.y)).y + 
                        texture(velocityTexture, texCoords + vec2(0.0f, texelSize.y)).y;
    fragColor = vec2(divergence, 0.0f);
}
