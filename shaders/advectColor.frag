#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D velocityTexture;
uniform vec2 texelSize;
uniform float dt;

out vec3 fragColor;
in vec2 texCoords;

void main() {
    vec2 velocity = texture(velocityTexture, texCoords).xy;
    vec2 prevPos = texCoords - dt * velocity * texelSize;
    fragColor = texture(colorTexture, prevPos).xyz * 0.995;
    //fragColor = texture(colorTexture, texCoords).xyz;// + vec3(0.003f, 0.003, 0.003f);
}
