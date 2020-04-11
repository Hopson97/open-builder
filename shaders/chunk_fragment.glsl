#version 330

in float passBasicLight;
in vec3 passTexCoord;

out vec4 outColour;

uniform sampler2DArray textureArray;

void main() {
    outColour = passBasicLight * texture(textureArray, passTexCoord);
    if (outColour.a == 0) {
        discard;
    }
}
