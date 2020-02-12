#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D depthTex;
uniform vec2 pixelSize;
uniform int isVertical;
uniform float focalPoint;
uniform int type;

in Vertex {
    vec2 texCoord;
    vec4 colour;
} IN;

out vec4 fragColour;

const float f=15000.0;
const float n = 1;

void main(void){

    float factor = 1;
    if(type == 1){
        float nearFocus = focalPoint - 0.1;
        float farFocus = focalPoint + 0.1;

        float z = (2 * n) / (f + n - texture( depthTex, IN.texCoord ).r * (f - n));
        if(z < farFocus && z > nearFocus) factor = 0;
        if(z > farFocus) factor = (z - farFocus) / (1-farFocus);
        if(z < nearFocus) factor = -(z - nearFocus) / nearFocus;
    }

    float weights[5] = float[](0.12, 0.22, 0, 0.22, 0.12);
    float remWeight = 1;
    for(int i=0; i<5; ++i) {
            weights[i] *= factor;
            remWeight -= weights[i];
    }
    weights[2] = remWeight;

    vec2 values[5];
    if(isVertical == 1){
        values = vec2[](
            vec2(0.0, -pixelSize.y*2),
            vec2(0.0, -pixelSize.y*1),
            vec2(0.0,0.0),
            vec2(0.0, pixelSize.y*1),
            vec2(0.0, pixelSize.y*2));
    }
    else {
        values = vec2[](
            vec2(-pixelSize.y*2, 0.0),
            vec2(-pixelSize.y*1, 0.0),
            vec2(0.0, 0.0),
            vec2(pixelSize.y*1, 0.0),
            vec2(pixelSize.y*2, 0.0));
    }

    fragColour = vec4(0,0,0,0);
    for(int i=0; i<5; ++i){
        vec4 temp = texture(diffuseTex, IN.texCoord.xy + values[i]);
        fragColour += temp * weights[i];
    }

}
