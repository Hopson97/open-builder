#version 330

layout (location = 0) in vec3 inVertexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 passTextureCoord;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(inVertexCoord, 1.0);
    
    passTextureCoord = inVertexCoord;

}
