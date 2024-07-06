/* Jogo de Tilemap
 *
 * Desenvolvido por Marcelo Strack Daros para a disciplina de Processamento Grafico - Unisinos
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h> 

using namespace std;

// Classes para manipulação dos shaders
#include "Shader.h"
#include "Sprite.h"
#include "CharacterController.h"
#include <vector>
#include <iostream>

// Protótipos das funções
GLuint loadTexture(string texturePath);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupRectangles();

// Dimensoes da janela
const GLuint WIDTH = 1500, HEIGHT = 768;

const int COLUMNS = 16, LINES = 15;
int rounds = 8;
int score = 0;
glm::vec3 colorSelected = glm::vec3(0.0f, 0.0f, 0.0f);

CharacterController* hero;

int main()
{
	cout << "Bem vindo ao Jogo Tilemap!" << endl;

	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo Tilemap", nullptr, nullptr);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funçães da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Definindo as dimensoes da viewport com as mesmas dimensoes da janela da aplicacao
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o shaders
	Shader shader("../shaders/retangulo.vs", "../shaders/retangulo.fs");
	Shader heroShader("../shaders/tex_vert.glsl", "../shaders/tex_frag.glsl");

	// Configuracao de camadas e combinacao de texturas
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);

	Sprite coin1(&shader, loadTexture("../images/coin.png"), glm::vec3(270, 285, 0.0), glm::vec3(50, 50, 1.0), 0.0, 0.0, false);
	Sprite coin2(&shader, loadTexture("../images/coin.png"), glm::vec3(1420, 384, 0.0), glm::vec3(50, 50, 1.0), 0.0, 0.0, false);
	Sprite coin3(&shader, loadTexture("../images/coin.png"), glm::vec3(750, 30, 0.0), glm::vec3(50, 50, 1.0), 0.0, 0.0, false);
	Sprite coin4(&shader, loadTexture("../images/coin.png"), glm::vec3(460, 585, 0.0), glm::vec3(50, 50, 1.0), 0.0, 0.0, false);
	hero = new CharacterController(&shader, loadTexture("../images/hero_spritesheet.png"), glm::vec3(750, 384, 0.0), glm::vec3(50, 50, 1.0), 0.0, 5.0, true, 3, 1);

	glActiveTexture(GL_TEXTURE0);
	heroShader.Use();
	heroShader.setInt("texBuffer", 0);

	// Matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::ortho(0.0, 1500.0, 0.0, 768.0, -1.0, 1.0);
	heroShader.setMat4("projection", glm::value_ptr(projection));

	// Gerando um buffer simples
	GLuint VAO = setupRectangles();
	glm::mat4 model = glm::mat4(1);

	// Array para armazenar as cores de cada retangulo
	int amount = COLUMNS * LINES;
	glm::vec3* colors = new glm::vec3[amount];

	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funcoes de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO); // Conectando ao buffer de geometria

		shader.Use();
		GLfloat xc = -0.77f,
			xl = 0.90f;
		int colorIndex = 0;

		for (int c = 0; c < COLUMNS; c++) {
			for (int l = 0; l < LINES; l++) {

				if (((l == 0 || l == 14) && c != 7) ||
					((l == 1 || l == 13) && c != 6 && c != 7 && c != 8) ||
					((l == 2 || l == 12) && c != 5 && c != 6 && c != 7 && c != 8 && c != 9) ||
					((l == 3 || l == 11) && c != 4 && c != 5 && c != 6 && c != 7 && c != 8 && c != 9 && c != 10) ||
					((l == 4 || l == 10) && c != 3 && c != 4 && c != 5 && c != 6 && c != 7 && c != 8 && c != 9 && c != 10 && c != 11) ||
					((l == 5 || l == 9) && c != 2 && c != 3 && c != 4 && c != 5 && c != 6 && c != 7 && c != 8 && c != 9 && c != 10 && c != 11 && c != 12) ||
					((l == 6 || l == 8) && c != 1 && c != 2 && c != 3 && c != 4 && c != 5 && c != 6 && c != 7 && c != 8 && c != 9 && c != 10 && c != 11 && c != 12 && c != 13) ||
					c == 15) {
					colors[colorIndex] = glm::vec3(0, 0, 0); //preto
				}
				else {
					colors[colorIndex] = glm::vec3(0, 0.4, 0); // verde escuro
				}

				if (((l == 4 || l == 5) && (c == 5 || c == 6)) ||
					((l == 6 || l == 7) && (c == 10 || c == 11)) ||
					((l == 10 || l == 11) && (c == 6 || c == 8))) {
					colors[colorIndex] = glm::vec3(0, 0, 0.6); // azul
				}

				if ((l == 11 || l == 12 || l == 10 || l == 9) && c == 7) {
					colors[colorIndex] = glm::vec3(0.5451, 0.2706, 0.0745); // marrom
				}

				shader.setVec3("cor", colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(xc + c * 0.12, xl - l * 0.11, 0));
				model = glm::scale(model, glm::vec3(0.38, 0.38, 1));
				shader.setMat4("model", glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				colorIndex = colorIndex + 1;
			}
		}

		// Desenho das texturas
		coin1.drawShader();
		coin2.drawShader();
		coin3.drawShader();
		coin4.drawShader();
		hero->drawShader();

		// Desconectando o buffer de geometria
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Libera a memoria reservada para as cores
	delete[] colors;
	// Desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execucao da GLFW, limpando os recursos alocados
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W) {
		hero->goUp();
		hero->changeStep();
	}

	if (key == GLFW_KEY_S) {
		hero->goDown();
		hero->changeStep();
	}

	if (key == GLFW_KEY_A) {
		hero->goLeft();
		hero->changeStep();
	}

	if (key == GLFW_KEY_D) {
		hero->goRight();
		hero->changeStep();
	}
}

// Configura��o e desenho dos vertices
int setupRectangles()
{
	GLfloat vertices[] = {
		-0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // V�rtice ret�ngulo
		-0.5f,  0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // V�rtice ret�ngulo
		 0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // V�rtice ret�ngulo
		 0.5f,  0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // V�rtice ret�ngulo 
		-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  // V�rtice cores (R)
		 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  // V�rtice cores (G)
		 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  // V�rtice cores (B)
		-0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f   // V�rtice cores (RGB)
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo 0 - posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

GLuint loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Configura��o do par�metro WRAPPING nas coords s e t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Confiruga��o do par�metro FILTERING na minifica��o e magnifica��o da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true); // comando para que a imagem n�o fique invertida
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}