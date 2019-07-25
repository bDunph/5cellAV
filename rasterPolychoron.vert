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

	float dist = 2.0;
	float xPos3D = position4D.x * dist / (dist - position4D.w);
	float yPos3D = position4D.y * dist / (dist - position4D.w);
	float zPos3D = position4D.z * dist / (dist - position4D.w);

	vec4 position3D = vec4(xPos3D, yPos3D, zPos3D, 1.0);

	fragPos_worldSpace = vec3(fiveCellModelMat * position3D).xyz;

	//project normal to 3D
	float xNorm3D = normal4D.x * dist / (dist - normal4D.w);
	float yNorm3D = normal4D.y * dist / (dist - normal4D.w);
	float zNorm3D = normal4D.z * dist / (dist - normal4D.w);
	
	vec3 normal3D = vec3(xNorm3D, yNorm3D, zNorm3D);
	vertNormal_worldSpace = mat3(transpose(inverse(fiveCellModelMat))) * normal3D;

	gl_Position = projMat * viewMat * fiveCellModelMat *  position3D;	
}
