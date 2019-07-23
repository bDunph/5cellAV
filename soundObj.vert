#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 scaleMat;

out vec3 fragPos_worldSpace;
out vec3 normal_worldSpace;

void main(){
	
	mat4 transMat = mat4(2.0, 2.0, 2.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);

	gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);	

	fragPos_worldSpace = vec3(modelMat * vec4(position, 1.0)).xyz;  	
	normal_worldSpace = mat3(transpose(inverse(modelMat))) * normal;

}
