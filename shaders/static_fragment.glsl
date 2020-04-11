#version 330

in vec3 passNormal;
in vec3 passFragPosition;
in vec2 passTexCoord;

out vec4 outColour;

uniform vec3 lightPosition;
uniform sampler2D tex;

void main() {
    vec3 distanceToLight = lightPosition - passFragPosition;
    float dist = length(distanceToLight);
    float attenuation = min(6.0 / dist, 1.0);
    
    vec3 lightDirection = normalize(lightPosition - passFragPosition);
    float difference = max(dot(passNormal, lightDirection), 0.2);

    //outColour = attenuation * difference * vec4(colour, 1.0) * texture(tex, passTexCoord);
    outColour = texture(tex, passTexCoord);
}
