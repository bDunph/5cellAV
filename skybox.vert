#version 410

layout (location = 0) in vec3 position;

uniform mat4 projMat;
uniform mat4 viewMat;

out vec3 TexCoords;

void main(){

	TexCoords = position;
	gl_Position = projMat * viewMat * vec4(position, 1.0);
}
