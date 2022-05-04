#version 410 core

in vec4 Color;
in vec2 Texcoord;

out vec4 FragColor;

uniform sampler2D Texture;

void main() {
    FragColor = Color * texture(Texture, Texcoord);
}
