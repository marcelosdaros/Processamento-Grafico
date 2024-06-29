#pragma once
#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Sprite
{
public:
	Sprite(Shader* shader, GLuint texID, glm::vec3 position, glm::vec3 scale, float angle);
	~Sprite();

	void setShader(Shader* shader);
	void drawShader();

protected:
	Shader* shader;
	GLuint VAO;
	GLuint texID;
	glm::vec3 position;
	glm::vec3 scale;
	float angle;

	void configureModel();
};

