#version 330

in vec2 passTexCoord;
out vec4 outColour;

uniform sampler2D bloomTexture;
uniform sampler2D colourTexture;

uniform bool bloomToggle;

void main() { 
    vec3 colour = texture(colourTexture, passTexCoord).rgb;
    if (bloomToggle) {
        colour += texture(bloomTexture, passTexCoord).rgb;
    }
    outColour = vec4(colour, 1.0);
}