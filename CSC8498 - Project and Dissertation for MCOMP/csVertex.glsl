#version 150

uniform vec4 nodeColour;

uniform float modelFloats[25];
uniform float viewFloats[25];
uniform float projFloats[25];
uniform float projFloats4D[25];

in vec4 position;
in vec4 colour;
in vec4 normal;
in vec2 texCoord;

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

mat4 toMat4(float[25] m5){
	mat4 result;
	for(int x=0; x<4; ++x){
		for(int y=0; y<4; ++y){
			result[x][y] = m5[5*x + y];
		}
	}
	return result;
}

void main(void) {

	float[25] vpMat = mulMatrices(projFloats, viewFloats);
	float[5] worldPos = mulVertex(modelFloats, toFloats(position));
	float[5] outPos = mulVertex(vpMat, worldPos);
	gl_Position.x = outPos[0];
	gl_Position.y = outPos[1];
	gl_Position.z = outPos[2];
	gl_Position.w = outPos[4];

	OUT.worldPos.x = worldPos[0];
	OUT.worldPos.y = worldPos[1];
	OUT.worldPos.z = worldPos[2];
	OUT.worldPos.w = worldPos[3];

	mat4 normalMatrix = transpose(inverse(toMat4(modelFloats)));
	OUT.normal = normalize(normalMatrix * normalize(normal));

	//float factor = (worldPos[3] + 1) / 2;  //-1 -> 1
	float factor = dot(OUT.normal, vec4(0,1,0,0));
	OUT.colour = colour;
	//OUT.colour = vec4(max(OUT.normal.xyz, 0), 1);

}
