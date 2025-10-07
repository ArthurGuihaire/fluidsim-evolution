#version 330 core

in vec2 texCoords;
uniform vec3 inputColor;
uniform float drawRadiusSquared;
uniform vec2 drawCenter;
uniform vec2 texelSize;
out vec3 fragColor;

void main() {
    vec2 normCoords = texCoords * 2.0f - vec2(1.0f, 1.0f);
    vec2 deltaNorm = normCoords - drawCenter;                   // difference in [-1, 1] space
    vec2 deltaPixels = deltaNorm * 0.5 / texelSize;             // convert to pixel units
    float distanceSquared = dot(deltaPixels, deltaPixels);
    if (distanceSquared < drawRadiusSquared) {
        float colorDivider = 1 + (distanceSquared / drawRadiusSquared);
        fragColor = inputColor / (colorDivider * 256);   //Add one to avoid divide by zero
    }
    else {
        discard;
    }
}
