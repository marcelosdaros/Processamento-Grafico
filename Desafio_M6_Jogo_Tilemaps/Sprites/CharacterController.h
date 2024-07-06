#pragma once
#include "Sprite.h""
#include <iostream>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CharacterController : public Sprite {

public:
	CharacterController(Shader* shader, GLuint texID, glm::vec3 position, glm::vec3 scale, float angle, float movement, bool mirror, int nFrames, int step);
	~CharacterController();
	void changeStep();
	void setupVertices(float pi, float pf);

protected:
	int nFrames;
	int step;
};