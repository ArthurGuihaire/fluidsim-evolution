#version 330 core

layout(location=0) in vec2 position;
//layout(location=1) in vec4 color;
//layout(location=2) in float smoothingFactor;

//flat out vec4 instanceColor;
//flat out vec4 smoothing;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    //instanceColor = color;
    //smoothing = smoothingFactor;
}