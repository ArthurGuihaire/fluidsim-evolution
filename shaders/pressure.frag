#version 330 core

uniform sampler2D divPresTexture; //Divergence in R channel, pressure in G channel
uniform vec2 texelSize;

out vec2 fragColor;
in vec2 texCoords;

void main() {
    float neighborPressureSum = 
        texture(divPresTexture, texCoords - vec2(texelSize.x, 0.0f)).g +
        texture(divPresTexture, texCoords + vec2(texelSize.x, 0.0f)).g +
        texture(divPresTexture, texCoords - vec2(0.0f, texelSize.y)).g + 
        texture(divPresTexture, texCoords + vec2(0.0f, texelSize.y)).g;

    float divergence = texture(divPresTexture, texCoords).r;

    fragColor = vec2(divergence, (neighborPressureSum - divergence) / 4);
}
