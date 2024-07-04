#include "CharacterController.h"

CharacterController::CharacterController(Shader* shader, GLuint texID, glm::vec3 position, glm::vec3 scale, float angle, float movement, int nFrames, int step)
	: Sprite(shader, texID, position, scale, angle, movement) {
	
	this->nFrames = nFrames;
	this->step = step;
	
	setupVertices(0.33, 0.0);
}

CharacterController::~CharacterController() {
	glDeleteVertexArrays(1, &VAO);
}

void CharacterController::setupVertices(float pi, float pf) {
	// Coordenadas x, y e z dos triângulos para mandar para o VBO (Vertex Buffer Object)
	// Os atributos foram armazenados num VBO único
	GLfloat vertices[] = {
		// x    y     z     R     G     B     s   t
		// Triangulo 0
		 -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, pi, 1.0f,  //v0
		 -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, pi, 0.0f,  //v1
		  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, pf, 1.0f,  //v2
		// Triangulo 1	
		 -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, pi, 0.0f,  //v1
		  0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, pf, 0.0f,  //v3
		  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, pf, 1.0f   //v2

	};

	GLuint VBO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo 2 - coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Desvincula o VAO para evitar bugs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CharacterController::changeStep() {
	if (this->step == 3) {
		this->step = 1;
	}
	else {
		this->step++;
	}

	float interval = 1.0 / (float)this->nFrames;
	float pf = interval * (float)this->step; // ponto final
	float pi = pf - 0.33; // ponto inicial
	setupVertices(pi, pf);
}