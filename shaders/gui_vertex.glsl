#version 330

layout (location = 0) in vec2 inVertexCoord;
layout (location = 1) in vec2 inTextureCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 passTexCoord;

void main() {
    gl_Position = projectionMatrix * modelMatrix * vec4(inVertexCoord, 0.0, 1.0);
    passTexCoord = inTextureCoord;
}
