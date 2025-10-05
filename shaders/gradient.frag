#version 330 core

in vec2 texCoords;
uniform sampler2D divPresTexture; // We only care about pressure (G channel)
uniform sampler2D velocityTexture;
uniform vec2 texelSize;
out vec2 newVelocity;

void main() {
    vec2 delPressure = vec2(
        texture(divPresTexture, texCoords + vec2(texelSize.x, 0.0f)).g -
        texture(divPresTexture, texCoords - vec2(texelSize.x, 0.0f)).g, 
        texture(divPresTexture, texCoords + vec2(0.0f, texelSize.y)).g - 
        texture(divPresTexture, texCoords - vec2(0.0f, texelSize.y)).g
    );
    //delPressure is double what it should be, since we sampled pixels 2 pixels away
    //Subtract half delPressure from velocity
    newVelocity = texture(velocityTexture, texCoords).xy - delPressure / 2;
}
