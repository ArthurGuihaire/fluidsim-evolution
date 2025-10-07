#version 330 core

uniform sampler2D velocityTexture;
uniform vec2 texelSize;

out vec2 fragColor;
in vec2 texCoords;

void main() {
    // Central differences scaled by grid spacing
    float divergence = texture(velocityTexture, texCoords + vec2(texelSize.x, 0.0f)).x -
                       texture(velocityTexture, texCoords - vec2(texelSize.x, 0.0f)).x + 
                       texture(velocityTexture, texCoords + vec2(0.0f, texelSize.y)).y -
                       texture(velocityTexture, texCoords - vec2(0.0f, texelSize.y)).y;
    //Divergence over 2 because we sampled pixels 2 away
    fragColor = vec2(divergence / 2, 0.0f);
}
