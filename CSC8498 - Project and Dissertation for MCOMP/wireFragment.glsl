#version 150 core

uniform vec4 nodeColour;

in Vertex{
	vec4 colour;
	vec4 worldPos;
	vec4 normal;
} IN;

out vec4 fragColour;

void main(void){
	fragColour = IN.colour * nodeColour;
}
