#version 330

layout (location = 0) in vec3 inVertexCoord;
layout (location = 1) in vec3 inTextureCoord;
layout (location = 2) in float inBasicLight;

uniform mat4 projectionViewMatrix;

out vec3 passTexCoord;
out float passBasicLight;

void main() {
    gl_Position = projectionViewMatrix * vec4(inVertexCoord, 1.0);
    
    passTexCoord = inTextureCoord;
    passBasicLight = inBasicLight;
}