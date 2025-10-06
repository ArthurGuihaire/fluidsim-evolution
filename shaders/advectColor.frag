#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D velocityTexture;
uniform vec2 texelSize;
uniform float dt;

out vec3 fragColor;
in vec2 texCoords;

void main() {
    vec2 velocity = texture(velocityTexture, texCoords).xy;
    vec2 prevPos = texCoords - dt * velocity * texelSize;
    fragColor = (texture(colorTexture, prevPos).xyz +
        texture(colorTexture, prevPos - vec2(texelSize.x * dt, 0.0f)).xyz +
        texture(colorTexture, prevPos + vec2(texelSize.x * dt, 0.0f)).xyz +
        texture(colorTexture, prevPos - vec2(0.0f, texelSize.y * dt)).xyz +
        texture(colorTexture, prevPos + vec2(0.0f, texelSize.y * dt)).xyz) * 0.995;
}
