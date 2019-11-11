#version 330

in float passBasicLight;
in vec3 passFragPosition;
in vec2 passTexCoord;

out vec4 outColour;

uniform sampler2D tex;

void main() {
    outColour = passBasicLight * texture(tex, passTexCoord);
}