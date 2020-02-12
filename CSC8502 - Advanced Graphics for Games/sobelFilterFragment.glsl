#version 150 core

uniform sampler2D diffuseTex;
uniform vec2 pixelSize;
uniform int isVertical;
uniform int type;

in Vertex {
    vec2 texCoord;
    vec4 colour;
} IN;

out vec4 fragColour;

float intensity(vec3 colour){
    return sqrt((colour.x*colour.x) + (colour.y*colour.y) + (colour.z*colour.z));
}

void main(void){

    if(isVertical == 0){
        vec3 values[9];
        for(int i=0; i<9; ++i){
            vec2 pos = vec2(
                pixelSize.x * ((i % 3) - 1),
                pixelSize.y * ((i / 3) - 1));
            values[i] = texture(diffuseTex, IN.texCoord + pos).rgb;
            if(type == 0){
                float colour = intensity(values[i]);
                values[i] = vec3(colour,colour,colour);
            }
        }

        vec3 x = (values[0] + 2*values[3] + values[6])
                - (values[2] + 2*values[5] + values[8]);

        vec3 y = (values[0] + 2*values[1] + values[2])
                - (values[6] + 2*values[7] + values[8]);

        vec3 colour = sqrt(x*x + y*y);
        fragColour = vec4(colour, 1);
    }
    else {
        fragColour = texture(diffuseTex, IN.texCoord);
    }




}
