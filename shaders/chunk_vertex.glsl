#version 330

layout (location = 0) in uint inVertexData;

uniform vec3 chunkPosition;

uniform mat4 projectionViewMatrix;

out vec3 passTexCoord;
out float passBasicLight;

vec2 texCoords[4] = vec2[4](
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f),
    vec2(1.0f, 1.0f),
    vec2(0.0f, 1.0f)
);

void main() {   
    float x = float(inVertexData & 0x3Fu);
    float y = float((inVertexData & 0xFC0u) >> 6u);
    float z = float((inVertexData & 0x3F000u) >> 12u);
    x += chunkPosition.x;
    y += chunkPosition.y;
    z += chunkPosition.z;
    gl_Position = projectionViewMatrix * vec4(x, y, z, 1.0);

    //Texture coords
    uint index = (inVertexData & 0x600000u) >> 21u;
    uint layer = (inVertexData & 0xFF800000u) >> 23u;
    
    passTexCoord = vec3(texCoords[index], float(layer));
    passBasicLight = float((inVertexData & 0x1C0000u) >> 18u) / 5.0f;
}
    