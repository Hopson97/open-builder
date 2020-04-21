#version 330

out vec4 outColour;
in vec3 passTextureCoord;

uniform samplerCube texSampler;

void main()
{
    outColour = texture(texSampler, passTextureCoord);
    //color = vec4(brightnessContrast(color.xyz, 1.15f, 1.15f), color.w);
    //color = vec4(gamma(color.xyz, 4.8f),color.w);

    //outColour = color;
    //if (outColour.a == 0) discard;
}
