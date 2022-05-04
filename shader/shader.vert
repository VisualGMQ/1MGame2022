#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec4 aColor;

out vec4 Color;
out vec2 Texcoord;

uniform mat4 Project;
uniform mat4 View;
uniform mat4 Model;

void main() {
    Texcoord = aTexcoord;
    Color = aColor;
    gl_Position = Project * View * Model * vec4(aPos, 0, 1);
}
