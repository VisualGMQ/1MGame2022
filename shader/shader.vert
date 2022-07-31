#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexcoord;

out vec2 Texcoord;

uniform mat4 Project;

void main() {
    Texcoord = aTexcoord;
    gl_Position = Project * vec4(aPos, 0, 1);
}
