#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D depthTex;
uniform vec2 pixelSize;
uniform int isVertical;
uniform int type;

in Vertex {
    vec2 texCoord;
    vec4 colour;
} IN;

out vec4 fragColour;

const float n = 1;
const float f = 15000;

float intensity(vec3 colour){
    return sqrt((colour.x*colour.x) + (colour.y*colour.y) + (colour.z*colour.z));
}

float getDepthValue(vec2 texCoord){
    return (2 * n) / (f + n - texture( depthTex, texCoord ).r * (f - n));
}

vec3 sobelDepthFilter(vec2 texCoord){
        vec3 values[9];
        for(int i=0; i<9; ++i){
            vec2 pos = vec2(
                pixelSize.x * ((i % 3) - 1),
                pixelSize.y * ((i / 3) - 1));
            float colour = getDepthValue(texCoord + pos);
            values[i] = vec3(colour,colour,colour);
        }

        vec3 x = (values[0] + 2*values[3] + values[6])
                - (values[2] + 2*values[5] + values[8]);

        vec3 y = (values[0] + 2*values[1] + values[2])
                - (values[6] + 2*values[7] + values[8]);

        return sqrt(x*x + y*y);
}

void main(void){

    fragColour = texture(diffuseTex, IN.texCoord);
    if(isVertical == 0){
        vec3 outline = sobelDepthFilter(IN.texCoord);
        if(outline.r > 0.05)
            fragColour = vec4(0,0,0,1);
    }


}
