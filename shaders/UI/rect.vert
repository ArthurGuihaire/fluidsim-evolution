#version 330 core

layout(location=0) in vec4 boundingBox;
layout(location=1) in float roundingRadius;
layout(location=2) in vec4 color;

flat out vec2 rectMin;
flat out vec2 rectSize;

flat out vec4 instanceColor;
flat out float radius;

uniform vec2 windowSize;

void main() {
    uint xIndex = gl_VertexID & 0x00000001;
    uint yIndex = (gl_VertexID & 0x00000002) + 1;
    vec2 position = vec2(boundingBox[xIndex], boundingBox[yIndex]);
    gl_Position = vec4(position, 0.0f, 1.0f);

    rectMin = boundingBox.xy * windowSize;
    vec2 rectMax = boundingBox.zw * windowSize;
    rectSize = rectMax - rectMin;

    instanceColor = color;
    radius = roundingRadius;
}
