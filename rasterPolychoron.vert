#version 410

layout(location = 0) in vec4 position4D;
layout(location = 1) in vec4 normal4D;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 fiveCellModelMat;
uniform mat4 rotZW;
 
out vec3 vertNormal_worldSpace;
out vec3 fragPos_worldSpace;

void main() {

	//rotate 4D point
	vec4 rotatedPos4D = rotZW * position4D;
	vec4 rotatedNorm4D = rotZW * normal4D;
	
	//stereographic projection
	float dist = 2.0;
	float xPos3D = rotatedPos4D.x * dist / (dist - rotatedPos4D.w);
	float yPos3D = rotatedPos4D.y * dist / (dist - rotatedPos4D.w);
	float zPos3D = rotatedPos4D.z * dist / (dist - rotatedPos4D.w);

	vec4 position3D = vec4(xPos3D, yPos3D, zPos3D, 1.0);

	fragPos_worldSpace = vec3(fiveCellModelMat * position3D).xyz;

	//project normal to 3D
	float xNorm3D = rotatedNorm4D.x * dist / (dist - rotatedNorm4D.w);
	float yNorm3D = rotatedNorm4D.y * dist / (dist - rotatedNorm4D.w);
	float zNorm3D = rotatedNorm4D.z * dist / (dist - rotatedNorm4D.w);
	
	vec3 normal3D = vec3(xNorm3D, yNorm3D, zNorm3D);
	vertNormal_worldSpace = mat3(transpose(inverse(fiveCellModelMat))) * normal3D;

	gl_Position = projMat * viewMat * fiveCellModelMat *  position3D;	
}
