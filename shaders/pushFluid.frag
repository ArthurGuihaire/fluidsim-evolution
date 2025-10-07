#version 330 core

in vec2 texCoords;
uniform vec2 pushForce;
uniform sampler2D velocityTexture;
uniform float pushRadiusSquared;
uniform vec2 pushCenter;
uniform vec2 texelSize;
out vec2 fragColor;

void main() {
    vec2 normCoords = texCoords * 2.0f - vec2(1.0f, 1.0f);
    vec2 deltaNorm = normCoords - pushCenter;               // difference in [-1, 1] space
    vec2 deltaPixels = deltaNorm * 0.5 / texelSize;         // convert to pixel units
    float distanceSquared = dot(deltaPixels, deltaPixels);
    fragColor = texture(velocityTexture, texCoords).xy;
    if (distanceSquared < pushRadiusSquared) {
        fragColor += pushForce / (distanceSquared + 1000.0f);  //Add one to avoid divide by zero
    }
}
