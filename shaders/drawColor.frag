#version 330 core

//in vec2 texCoords;
uniform vec3 inputColor;
//uniform sampler2D colorTexture;
out vec3 fragColor;

void main() {
    //fragColor = texture(colorTexture, texCoords).xyz + inputColor;
    fragColor = inputColor;
}
