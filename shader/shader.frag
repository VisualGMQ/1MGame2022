#version 330 core

in vec2 Texcoord;

out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D Texture;

void main() {
    FragColor = Color * texture(Texture, Texcoord);
}
