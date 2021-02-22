#version 330 core

in vec2 passTexCoord;
out vec4 outColour;

uniform sampler2D tex;

uniform bool isHorizontalBlur;

const float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
void main()
{             
    vec2 texOffset = 1.0 / textureSize(tex, 0); // gets size of single texel
    vec3 result = texture(tex, passTexCoord).rgb * weight[0];

    if (isHorizontalBlur) {
        for(int i = 1; i < 5; ++i) {
            result += texture(tex, passTexCoord + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
            result += texture(tex, passTexCoord - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else {
       for(int i = 1; i < 5; ++i) {
            result += texture(tex, passTexCoord + vec2(0.0, texOffset.y * i)).rgb * weight[i];
            result += texture(tex, passTexCoord - vec2(0.0, texOffset.y * i)).rgb * weight[i];
        }
    }

    outColour = vec4(result, 1.0);
}