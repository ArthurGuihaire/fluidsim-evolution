#version 330 core

in vec2 texCoords;
uniform sampler2D divPresTexture; // We only care about pressure (G channel)
uniform sampler2D velocityTexture;
uniform vec2 texelSize;
out vec2 newVelocity;

void main() {
    // Central differences with proper scaling
    vec2 gradP = vec2(
        texture(divPresTexture, texCoords + vec2(texelSize.x, 0.0f)).g -
        texture(divPresTexture, texCoords - vec2(texelSize.x, 0.0f)).g, 
        texture(divPresTexture, texCoords + vec2(0.0f, texelSize.y)).g -
        texture(divPresTexture, texCoords - vec2(0.0f, texelSize.y)).g);
    // Apply damping to prevent explosion
    newVelocity = texture(velocityTexture, texCoords).xy - gradP * 0.5f;
}
