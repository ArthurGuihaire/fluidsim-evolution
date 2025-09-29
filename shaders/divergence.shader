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
    vec2 force = vec2(
        texture(inputTexture, texCoords - vec2(texelSize.x, 0.0f)).z -
        texture(inputTexture, texCoords + vec2(texelSize.x, 0.0f)).z,
        texture(inputTexture, texCoords - vec2(0.0f, texelSize.y)).y -
        texture(inputTexture, texCoords + vec2(0.0f, texelSize.y)).y);

    vec3 texel = texture(inputTexture, texCoords).xyz;

    fragColor = vec3(texel.xy + force, texel.z);
}
