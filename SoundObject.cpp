#include "SoundObject.hpp"
#include "shader_manager.h"

#include <GLFW/glfw3.h>

bool SoundObject::setup(){

		//Sound source vertices
	float soundVerts [24] = {
		//top left front	
		-1.0f, 1.0f, 1.0f,
		//bottom left front
		-1.0f, -1.0f, 1.0f,
		//bottom right front
		1.0f, -1.0f, 1.0f,
		//top right front
		1.0f, 1.0f, 1.0f,
		//top left back
		-1.0f, 1.0f, -1.0f,
		//bottom left back
		-1.0f, -1.0f, -1.0f,
		//bottom right back
		1.0f, -1.0f, -1.0f,
		//top right back
		1.0f, 1.0f, -1.0f
	};

	unsigned int soundIndices [36] = {
		//front face
		0, 1, 2,
		0, 2, 3,
		//right face
		3, 2, 6,
		3, 6, 7,
		//back face
		7, 6, 5,
		7, 5, 4,
		//left face
		4, 5, 1,
		4, 1, 0,
		//bottom face
		1, 5, 6,
		1, 6, 2,
		//top face
		4, 0, 3,
		4, 3, 7
	};

	float soundObjNormals [24] = {
		//top front left
		-1.0f, 1.0f, 1.0f,
		//bottom front left
		-1.0f, -1.0f, 1.0f,
		//bottom front right
		1.0f, -1.0f, 1.0f,
		//top front right
		1.0f, 1.0f, 1.0f,
		//top left back
		-1.0f, 1.0f, -1.0f,
		//bottom left back
		-1.0f, -1.0f, -1.0f,
		//bottom right back
		1.0f, -1.0f, -1.0f,
		//top right back
		1.0f, 1.0f, -1.0f
	};	

	//Set up ground plane buffers
	glGenVertexArrays(1, &soundVAO);
	glBindVertexArray(soundVAO);

	GLuint soundVBO;
	glGenBuffers(1, &soundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, soundVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), soundVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint soundObjNormalVBO;
	glGenBuffers(1, &soundObjNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, soundObjNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), soundObjNormals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &soundObjIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, soundObjIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), soundIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//load shaders
	char* soundObjVertShader;
	bool isSoundObjVertLoaded = load_shader("soundObj.vert", soundObjVertShader);
	if(!isSoundObjVertLoaded) return 1;

	char* soundObjFragShader;
	bool isSoundObjFragLoaded = load_shader("soundObj.frag", soundObjFragShader);
	if(!isSoundObjFragLoaded) return 1;

	GLuint svs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(svs, 1, &soundObjVertShader, NULL);
	glCompileShader(svs);
	delete[] soundObjVertShader;
	//check for compile errors
	bool isSoundObjVertCompiled = shader_compile_check(svs);
	if(!isSoundObjVertCompiled) return 1;

	GLuint sfs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sfs, 1, &soundObjFragShader, NULL);
	glCompileShader(sfs);
	delete[] soundObjFragShader;
	//check for compile errors
	bool isSoundObjFragCompiled = shader_compile_check(sfs);
	if(!isSoundObjFragCompiled) return 1;
	
	soundObjShaderProg = glCreateProgram();
	glAttachShader(soundObjShaderProg, sfs);
	glAttachShader(soundObjShaderProg, svs);
	glLinkProgram(soundObjShaderProg);
	bool didSoundObjShadersLink = shader_link_check(soundObjShaderProg);
	if(!didSoundObjShadersLink) return 1;

	//uniform setup
	soundObj_projMatLoc = glGetUniformLocation(soundObjShaderProg, "projMat");
	soundObj_viewMatLoc = glGetUniformLocation(soundObjShaderProg, "viewMat");
	soundObj_modelMatLoc = glGetUniformLocation(soundObjShaderProg, "soundModelMat");

	soundObj_lightPosLoc = glGetUniformLocation(soundObjShaderProg, "lightPos");
	soundObj_light2PosLoc = glGetUniformLocation(soundObjShaderProg, "light2Pos");

	soundObj_cameraPosLoc = glGetUniformLocation(soundObjShaderProg, "camPos");
	
	//only use during development as computationally expensive
	bool validSoundObjProgram = is_valid(soundObjShaderProg);
	if(!validSoundObjProgram){
		fprintf(stderr, "ERROR: soundObjShaderProg not valid\n");
		return 1;
	}

	glBindVertexArray(0);
	
	identityModelMat = glm::mat4(1.0);
	glm::vec3 trans = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);
	scaleMat = glm::scale(identityModelMat, scale);
	translateMat = glm::translate(identityModelMat, trans);

}

void SoundObject::update(){

	float soundModelRotAngle = glfwGetTime() * 0.2f;
	glm::mat4 rotateSoundModel = glm::rotate(modelMatrix, soundModelRotAngle, glm::vec3(0, 1, 0));;
	soundModelMatrix = translateMat * rotateSoundModel * scaleMat;
}

void SoundObject::draw(){

	glEnable(GL_CULL_FACE);
	glBindVertexArray(soundVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, soundObjIndexBuffer);
	glUseProgram(soundObjShaderProg);

	glUniformMatrix4fv(soundObj_projMatLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(soundObj_viewMatLoc, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(soundObj_modelMatLoc, 1, GL_FALSE, &soundModelMatrix[0][0]);
	glUniform3f(soundObj_lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(soundObj_light2PosLoc, lightPos2.x, lightPos2.y, lightPos2.z);
	glUniform3f(soundObj_cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

	glDrawElements(GL_TRIANGLES, 36 * sizeof(unsigned int), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
