#version 330 core

in vec2 texCoords;
uniform vec2 pushForce;
uniform sampler2D velocityTexture;
uniform float dt;
out vec2 fragColor;

void main() {
    fragColor = texture(velocityTexture, texCoords).xy + pushForce;// * dt;
}
