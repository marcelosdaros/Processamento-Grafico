#pragma once
#include "Sprite.h""
#include <iostream>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CharacterController : public Sprite {

public:
	CharacterController(Shader* shader, GLuint texID, glm::vec3 position, glm::vec3 scale, float angle, float movement, int nFrames, int iFrame);
	~CharacterController();

protected:
	int nFrames;
	int iFrame;
};