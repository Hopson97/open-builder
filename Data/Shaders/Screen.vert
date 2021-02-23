/**
    Used for rendering the screen
*/
#version 330

vec2 vertexCoord[4] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
);

vec2 textureCoord[4] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0)
);

int indices[6] = int[](
    0, 1, 2, 2, 1, 3
);

out vec2 passTexCoord;

void main() {
    int i = indices[gl_VertexID];
    gl_Position = vec4(vertexCoord[i], 0.0, 1.0);
    passTexCoord = textureCoord[i];
}
