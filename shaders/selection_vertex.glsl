#version 330

layout (location = 0) in vec3 inVertexCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

void main() {
    gl_Position = projectionViewMatrix * modelMatrix * vec4(inVertexCoord, 1.0);
}
