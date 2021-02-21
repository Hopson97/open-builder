#version 330

layout (location = 0) out vec4 outColour;
//layout (location = 1) out vec4 outBrightColour;

//uniform vec3 lightPosition;
//uniform vec3 colour;

//in vec3 passFragPosition;
//in vec3 passNormal;

void main() {
    outColour = vec4(1.0, 0.5, 0.5, 1.0);
    //vec3 normal = normalize(passNormal);
    //vec3 lightDirection = normalize(lightPosition - passFragPosition);

    //float diff = max(dot(normal, lightDirection) * 1.3, 0.3);
    //vec3 diffuse = colour * diff * 1.1;
    
    //outColour = vec4(diffuse, 1.0f);

    //float brightness = (colour.r * 1.5) + (colour.g * 0.5) + (colour.b * 0.5);
    //if(brightness > 0.6)
    ///    outBrightColour = vec4(outColour.rgb, 1.0);
    //else
    //    outBrightColour = vec4(0.0, 0.0, 0.0, 1.0); 
}