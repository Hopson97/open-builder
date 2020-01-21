#version 330

in vec2 passTexCoord;
out vec4 outColour;

uniform sampler2D tex;
uniform vec3 color3;

void main() {
    outColour = texture(tex, passTexCoord) * vec4(color3.rgb, 1);
    if (outColour.a == 0) {
        discard;
    }
}