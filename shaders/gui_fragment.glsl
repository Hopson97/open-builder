#version 330

in vec2 passTexCoord;
out vec4 outColour;

uniform sampler2D tex;
uniform vec3 colour;

void main() {
    outColour = texture(tex, passTexCoord) * vec4(colour, 1.0);
    if (outColour.a == 0) {
        discard;
    }
}
