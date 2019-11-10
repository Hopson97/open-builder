#version 330

layout (location = 0) in vec3 inVertexCoord;
layout (location = 1) in vec2 inTextureCoord;
layout (location = 2) in vec3 inNormalCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec2 passTexCoord;
out vec3 passNormal;
out vec3 passFragPosition;
out vec4 passWorldPosition;

void main() {
    vec4 worldPosition = modelMatrix * vec4(inVertexCoord, 1.0);
    gl_Position = projectionViewMatrix * worldPosition;
    
    passTexCoord = inTextureCoord;
    passNormal = inNormalCoord;
    passWorldPosition = worldPosition;
    
    passFragPosition = vec3(modelMatrix * vec4(inVertexCoord, 1.0));
}