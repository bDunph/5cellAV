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
	void update(glm::vec3 &translationVal);
	void draw(glm::mat4 &projMat, glm::mat4 &viewMat, glm::vec3 &lightPosition, glm::vec3 &light2Position, glm::vec3 cameraPosition);
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
	glm::mat4 soundModelMatrix;
	
};
#endif
