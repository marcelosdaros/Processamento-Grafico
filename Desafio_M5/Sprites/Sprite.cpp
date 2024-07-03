#include "Sprite.h"

Sprite::Sprite(Shader* shader, GLuint texID, glm::vec3 position, glm::vec3 scale, float angle, float movement) {

	this->texID = texID;
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->movement = movement;
	setShader(shader);

	// Coordenadas x, y e z dos triângulos para mandar para o VBO (Vertex Buffer Object)
	// Os atributos foram armazenados num VBO único
	GLfloat vertices[] = {
		// x    y     z     R     G     B     s     t
		// Triangulo 0
		 -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  //v0
		 -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //v1
		  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //v2
		  // Triangulo 1	
		   -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //v1
			0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //v3
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f   //v2

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

Sprite::~Sprite() {
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::setShader(Shader* shader) {
	this->shader = shader;
}

void Sprite::configureModel() {
	// Matriz das imagens de fundo (matriz de modelo)
	glm::mat4 model = glm::mat4(1); // Matriz identidade
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void Sprite::drawShader() {

	configureModel();

	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO); // Conectando ao buffer de geometria
	glDrawArrays(GL_TRIANGLES, 0, 6); // Para os triângulos

	glBindTexture(GL_TEXTURE_2D, 0); // unbind
	glBindVertexArray(0); // Desconectando o buffer de geometria
}

void Sprite::goUp() {
	position.y += movement;
}

void Sprite::goDown() {
	position.y -= movement;
}

void Sprite::goLeft() {
	position.x -= movement;
}

void Sprite::goRight() {
	position.x += movement;
}