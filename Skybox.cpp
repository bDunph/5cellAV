#include "Skybox.hpp"
#include "shader_manager.h"

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Skybox::setup(){

	//Skybox vertices
	float skyboxVerts [24] = {
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

	unsigned int skyboxIndices [36] = {
		//front face
		0, 3, 2,
		0, 2, 1,
		//right face
		3, 7, 6,
		3, 6, 2,
		//back face
		7, 4, 5,
		7, 5, 6,
		//left face
		4, 0, 1,
		4, 1, 5,
		//bottom face
		1, 2, 6,
		1, 6, 5,
		//top face
		4, 7, 3,
		4, 3, 0
	};

	float skyboxNormals [24] = {
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

	//Set up skybox buffers
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	GLuint skyboxVBO;
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), skyboxVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint skyboxNormalVBO;
	glGenBuffers(1, &skyboxNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), skyboxNormals, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//setup texture buffer
	glGenTextures(1, &skyboxTexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexID);

	//load textures
	std::vector<std::string> textureNames;
	textureNames.push_back("misty_ft.tga");
	textureNames.push_back("misty_bk.tga");
	textureNames.push_back("misty_up.tga");
	textureNames.push_back("misty_dn.tga");
	textureNames.push_back("misty_rt.tga");
	textureNames.push_back("misty_lf.tga");

	//std::string name1 = texName.append("_rt.tga");
	//textureNames.push_back(name1);
	//std::string name2 = texName.append("_lf.tga");
	//textureNames.push_back(name2);
	//std::string name3 = texName.append("_up.tga");
	//textureNames.push_back(name3);
	//std::string name4 = texName.append("_dn.tga");
	//textureNames.push_back(name4);
	//std::string name5 = texName.append("_ft.tga");
	//textureNames.push_back(name5);
	//std::string name6 = texName.append("_bk.tga");
	//textureNames.push_back(name6);	

	int width, height, numChannels;
	unsigned char* data;
	for(GLuint i = 0; i < textureNames.size(); i++){
		std::cout << textureNames[i] << std::endl;
		data = stbi_load(textureNames[i].c_str(), &width, &height, &numChannels, 0);
		if(data){
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		} else {
			std::cout << "ERROR: Cubemap not loaded" << std::endl;
			stbi_image_free(data);	
			return false;	
		}
	}			
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  	

	glGenBuffers(1, &skyboxIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), skyboxIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//load shaders
	char* skyboxVertShader;
	bool isSkyboxVertLoaded = load_shader("skybox.vert", skyboxVertShader);
	if(!isSkyboxVertLoaded) return false;

	char* skyboxFragShader;
	bool isSkyboxFragLoaded = load_shader("skybox.frag", skyboxFragShader);
	if(!isSkyboxFragLoaded) return false;

	GLuint sky_vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sky_vs, 1, &skyboxVertShader, NULL);
	glCompileShader(sky_vs);
	delete[] skyboxVertShader;
	//check for compile errors
	bool isSkyboxVertCompiled = shader_compile_check(sky_vs);
	if(!isSkyboxVertCompiled) return false;

	GLuint sky_fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sky_fs, 1, &skyboxFragShader, NULL);
	glCompileShader(sky_fs);
	delete[] skyboxFragShader;
	//check for compile errors
	bool isSkyboxFragCompiled = shader_compile_check(sky_fs);
	if(!isSkyboxFragCompiled) return false;
	
	skyboxShaderProg = glCreateProgram();
	glAttachShader(skyboxShaderProg, sky_fs);
	glAttachShader(skyboxShaderProg, sky_vs);
	glLinkProgram(skyboxShaderProg);
	bool didSkyboxShadersLink = shader_link_check(skyboxShaderProg);
	if(!didSkyboxShadersLink) return false;

	//uniform setup
	skybox_projMatLoc = glGetUniformLocation(skyboxShaderProg, "projMat");
	skybox_viewMatLoc = glGetUniformLocation(skyboxShaderProg, "viewMat");

	//only use during development as computationally expensive
	bool validSkyboxProgram = is_valid(skyboxShaderProg);
	if(!validSkyboxProgram){
		fprintf(stderr, "ERROR: skyboxShaderProg not valid\n");
		return false;
	}

	glBindVertexArray(0);
	
	glm::vec3 trans = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::mat4 identityModelMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMat = glm::scale(identityModelMatrix, scale);
	glm::mat4 translateMat = glm::translate(identityModelMatrix, trans);
	glm::mat4 skyboxModelMatrix = translateMat * scaleMat;
	
	return true;
}

void Skybox::draw(glm::mat4 projMat, glm::mat4 viewMat){

	glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMat));

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glUseProgram(skyboxShaderProg);
	glUniformMatrix4fv(skybox_projMatLoc, 1, GL_FALSE, &projMat[0][0]);
	glUniformMatrix4fv(skybox_viewMatLoc, 1, GL_FALSE, &viewNoTranslation[0][0]);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxIndexBuffer); 
	glDrawElements(GL_TRIANGLES, 36 * sizeof(unsigned int), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}
