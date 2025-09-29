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
    float pressure = 
        texture(inputTexture, texCoords - vec2(texelSize.x, 0.0f)).x -
        texture(inputTexture, texCoords + vec2(texelSize.x, 0.0f)).x +
        texture(inputTexture, texCoords - vec2(0.0f, texelSize.y)).y -
        texture(inputTexture, texCoords + vec2(0.0f, texelSize.y)).y;

    vec3 texel = vec3(texture(inputTexture, texCoords));

    fragColor = vec3(texel.xy, texel.z + pressure);
}
