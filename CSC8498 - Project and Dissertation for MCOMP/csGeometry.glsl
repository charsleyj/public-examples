#version 150 core

uniform float viewFloats[25];
uniform float projFloats[25];

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 24) out;

in Vertex {
	vec4 colour;
	vec4 worldPos;
	vec4 normal;
} IN[];

out Vertex {
	vec4 colour;
	vec4 worldPos;
	vec4 normal;
} OUT;

float[25] mulMatrices(float a[25], float b[25]){

	float result[25];
	for(int r = 0; r < 5; ++r) {
		for(int c = 0; c < 5; ++c) {
			result[c + r*5] = 0.0;
			for(int i = 0; i < 5; ++i) {
				result[c + r*5] += a[c+i*5] * b[i+r*5];
			}
		}
	}
	return result;
}

float[5] mulVertex(float m[25], float v[5]){
	float result[5];
	result[0] = v[0]*m[0] + v[1]*m[5] + v[2]*m[10] + v[3]*m[15] + v[4]*m[20];
	result[1] = v[0]*m[1] + v[1]*m[6] + v[2]*m[11] + v[3]*m[16] + v[4]*m[21];
	result[2] = v[0]*m[2] + v[1]*m[7] + v[2]*m[12] + v[3]*m[17] + v[4]*m[22];
	result[3] = v[0]*m[3] + v[1]*m[8] + v[2]*m[13] + v[3]*m[18] + v[4]*m[23];
	result[4] = v[0]*m[4] + v[1]*m[9] + v[2]*m[14] + v[3]*m[19] + v[4]*m[24];
	return result;
}

float[5] toFloats(vec4 v){
	float result[5];
	result[0] = v.x;
	result[1] = v.y;
	result[2] = v.z;
	result[3] = v.w;
	result[4] = 1;
	return result;
}

float distanceFromPlane(vec4 point){
	return point.w;
}

void addExistingVertex(int idx){
	OUT.colour = IN[idx].colour;
	OUT.worldPos = IN[idx].worldPos;
	OUT.normal = IN[idx].normal;
	gl_Position = gl_in[idx].gl_Position;
	EmitVertex();
}

void addNewVertex(int a, int b, float t){

	vec4 pos = IN[a].worldPos + (IN[b].worldPos - IN[a].worldPos) * t;
	OUT.colour = IN[a].colour + (IN[b].colour - IN[a].colour) * t;
	OUT.worldPos = IN[a].worldPos + (IN[b].worldPos - IN[a].worldPos) * t;
	OUT.normal = IN[a].normal + (IN[b].normal - IN[a].normal) * t;

	float[25] vpMat = mulMatrices(projFloats, viewFloats);
	float[5] worldPos = toFloats(pos);
	float[5] outPos = mulVertex(vpMat, worldPos);
	gl_Position.x = outPos[0];
	gl_Position.y = outPos[1];
	gl_Position.z = outPos[2];
	gl_Position.w = outPos[4];
	EmitVertex();
}

void lineIntersection(int a, int b){
	vec4 posA = IN[a].worldPos;
	vec4 posB = IN[b].worldPos;
	float distA = distanceFromPlane(posA);
	float distB = distanceFromPlane(posB);

	bool onPlaneA = abs(distA) < 0.000001;
	bool onPlaneB = abs(distB) < 0.000001;

	if(onPlaneA) addExistingVertex(a);
	//if(onPlaneB) addExistingVertex(b);
	if(onPlaneA || onPlaneB) return;
	if(distA * distB > 0) return;

	float t = distA / (distA - distB);
	addNewVertex(a, b, t);
}

void triIntersection(int a, int b, int c){
	lineIntersection(a,b);
	lineIntersection(b,c);
	lineIntersection(c,a);
}

void main0(){
	addExistingVertex(0);
	addExistingVertex(1);
	addExistingVertex(2);
	addExistingVertex(3);
	addExistingVertex(0);
	addExistingVertex(2);
	addExistingVertex(3);
	addExistingVertex(1);
	//EndPrimitive();
}

void main() {
	//main0();
	triIntersection(0,1,2);
	triIntersection(0,1,3);
	triIntersection(0,2,3);
	triIntersection(1,2,3);
	EndPrimitive();
}
