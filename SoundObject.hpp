//***********************************************************************************************
// Sound Object
//***********************************************************************************************

#ifndef SOUNDOBJECT_HPP
#define SOUNDOBJECT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SoundObject {

public:
	bool setup();
	void update();
	void draw():
private:

	GLuint soundVAO;
	GLuint soundObjIndexBuffer;
	GLuint soundObjShaderProg;

	GLint soundObj_projMatLoc;
	GLint soundObj_viewMatLoc;
	GLint soundObj_modelMatLoc;
	GLint soundObj_lightPosLoc;
	GLint soundObj_light2PosLoc;
	GLint soundObj_cameraPosLoc;

	glm::mat4 identityModelMat;
	glm::mat4 scaleMat;
	glm::mat4 translateMat;
	glm::mat4 soundModelMatrix;
	
};
#endif
