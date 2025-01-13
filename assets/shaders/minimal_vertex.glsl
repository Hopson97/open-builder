#version 330

layout (location = 0) in vec2 inVertexCoord;
layout (location = 1) in vec2 inTextureCoord;

out vec2 passTexCoord;

void main() {
    gl_Position = vec4(inVertexCoord, 0.0, 1.0);
    passTexCoord = inTextureCoord;
}
