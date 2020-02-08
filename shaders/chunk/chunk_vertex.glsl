#version 330

layout (location = 0) in vec3 inVertexCoord;
layout (location = 1) in vec3 inTextureCoord;
layout (location = 2) in float inBasicLight;

uniform vec3 chunkPosition;

uniform mat4 projectionViewMatrix;

out vec3 passTexCoord;
out float passBasicLight;

void main() {
    float x = chunkPosition.x + inVertexCoord.x;
    float y = chunkPosition.y + inVertexCoord.y;
    float z = chunkPosition.z + inVertexCoord.z;
    gl_Position = projectionViewMatrix * vec4(x, y, z, 1.0);
    
    passTexCoord = inTextureCoord;
    passBasicLight = inBasicLight;
}