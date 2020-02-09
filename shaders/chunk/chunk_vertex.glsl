#version 330

layout (location = 0) in uint inVertexCoord;
layout (location = 1) in vec2 inTextureIndex;
layout (location = 2) in uint inTextureLayer;

uniform vec3 chunkPosition;

uniform mat4 projectionViewMatrix;

out vec3 passTexCoord;
out float passBasicLight;

void main() {   
    float x = float(inVertexCoord & 0x3Fu);
    float y = float((inVertexCoord & 0xFC0u) >> 6u);
    float z = float((inVertexCoord & 0x3F000u) >> 12u);
    x += chunkPosition.x;
    y += chunkPosition.y;
    z += chunkPosition.z;
    gl_Position = projectionViewMatrix * vec4(x, y, z, 1.0);
    
    passTexCoord = vec3(inTextureIndex, float(inTextureLayer));
    passBasicLight = float((inVertexCoord & 0x1C0000u) >> 18u) / 5.0f;
}