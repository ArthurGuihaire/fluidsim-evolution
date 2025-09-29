#shader vertex
#version 330 core

layout(location=0) in vec2 position;

uniform vec2 mousePos;

void main() {
    gl_Position = vec4(position + mousePos, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

uniform vec3 inputColor;
out vec3 fragColor;

void main() {
    fragColor = inputColor;
}
