#shader vertex
#version 330 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texPosition;

out vec2 texCoords;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    texCoords = texPosition;
}

#shader fragment
#version 330 core

uniform sampler2D inputTexture;
uniform vec2 texelSize;

out vec3 fragColor;
in vec2 texCoords;

void main() {
    vec2 velocity = texture(inputTexture, texCoords).xy;
    fragColor = texture(inputTexture, texCoords + texelSize * velocity).xyz;
}
