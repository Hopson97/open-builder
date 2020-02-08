#version 330

layout (location = 0) in vec3 inVertexCoord;
layout (location = 1) in vec3 inTextureCoord;
layout (location = 2) in float inBasicLight;

uniform vec3 chunkPosition;

uniform mat4 projectionViewMatrix;
uniform float time;

out vec3 passTexCoord;
out float passBasicLight;

vec4 createWaveOffset(vec3 position)
{
    position.y += sin((time + position.x) * 1.5) / 8.8f;
    position.y += cos((time + position.z) * 1.5) / 8.1f;
    position.y -= 0.22;
    return vec4(position, 1.0);
}

void main() {
    vec4 position = createWaveOffset(vec3(
         chunkPosition.x + inVertexCoord.x,
         chunkPosition.y + inVertexCoord.y,
         chunkPosition.z + inVertexCoord.z
    ));

    gl_Position = projectionViewMatrix * position;
    
    passTexCoord = inTextureCoord;
    passBasicLight = inBasicLight;
}