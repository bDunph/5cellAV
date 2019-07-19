#version 410

layout (location = 0) in vec3 position;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 scaleMat;

out vec3 fragPos_worldSpace;
out vec3 normal_worldSpace;

void main(){
	
	float scaledPosX = position.x * 100.0;
	float scaledPosZ = position.z * 100.0;
 
	gl_Position = projMat * viewMat * modelMat * vec4(scaledPosX, position.y, scaledPosZ, 1.0);;	
	//gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);

	fragPos_worldSpace = vec3(modelMat * vec4(position, 1.0)).xyz;  	
	vec3 normal = vec3(0.0, 1.0, 0.0);
	normal_worldSpace = mat3(transpose(inverse(modelMat))) * normal;

}
