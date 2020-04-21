#version 330

layout (location = 0) in vec3 inVertexCoord;
layout (location = 1) in vec2 inTextureCoord;
layout (location = 2) in vec3 inNormalCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec2 passTexCoord;
out vec3 passNormal;
out vec3 passFragPosition;

void main() {
	
    gl_Position = projectionViewMatrix * modelMatrix * vec4(inVertexCoord, 1.0);
    
    passTexCoord = inTextureCoord;
    passNormal = inNormalCoord;
    passFragPosition = vec3(modelMatrix * vec4(inVertexCoord, 1.0));
}
