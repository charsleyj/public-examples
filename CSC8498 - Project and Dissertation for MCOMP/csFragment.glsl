#version 150 core

uniform vec3 cameraPos;
uniform vec4 nodeColour;

uniform vec4 lightColour4D;
uniform vec4 lightPos4D;
uniform float lightRadius4D;

in Vertex {
	vec4 colour;
	vec4 worldPos;
	vec4 normal;
} IN;

out vec4 fragColour;

vec4 light(vec4 diffuse){
	vec4 incident = normalize(lightPos4D - IN.worldPos);
	float lambert = max(0.0, dot(incident, IN.normal));

	//attenuation
	float dist = length(lightPos4D - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius4D, 0.0, 1.0);

	//specularity
	vec4 viewDir = normalize(vec4(cameraPos,0) - IN.worldPos);
	vec4 halfDir = normalize(incident + viewDir);

	float rFactor = max(0.0, dot(halfDir, IN.normal));
	float sFactor = pow(rFactor, 10.0);

	//final
	vec3 colour = (diffuse.rgb * lightColour4D.rgb);
	colour += (lightColour4D.rgb * sFactor) * 0.33;
	vec4 finalColour = vec4(colour * atten * lambert, diffuse.a);
	finalColour.rgb += (diffuse.rgb * lightColour4D.rgb) * 0.15;
	return finalColour;
}

void main(void){
	vec4 diffuse = IN.colour * nodeColour;
	fragColour = light(diffuse);
}
