#version 330 core

uniform sampler2D velocityTexture;
uniform vec2 texelSize;
uniform float dt;

out vec2 fragColor;
in vec2 texCoords;

void main() {
    vec2 velocity = texture(velocityTexture, texCoords).xy;
    vec2 prevPos = texCoords - dt * velocity * texelSize;
    fragColor = texture(velocityTexture, prevPos).xy * 0.995;
}
