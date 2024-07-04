/* 
 * Desenvolvido por Marcelo Strack Daros para a disciplina de Processamento Gr�fico - Unisinos
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

//Classes manipula��o de shaders e sprites
#include "Shader.h"
#include "Sprite.h"
#include <vector>

// Prot�tipos das fun��es
GLuint loadTexture(string texturePath);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Dimens�es da janela
const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Texturas", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falha ao inicializar a GLAD" << endl;
	}

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/tex_vert.glsl", "../shaders/tex_frag.glsl");

	// Configura��o de camadas e combina��o de texturas
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);

	// Armazenamento do diret�rio de cada textura
	std::string textures[] = {
		"../battleground_images/sky.png",
		"../battleground_images/graves.png",
		"../battleground_images/back_trees.png",
		"../battleground_images/crypt.png",
		"../battleground_images/wall.png",
		"../battleground_images/ground.png",
		"../battleground_images/tree.png",
		"../battleground_images/bones.png",
		"../battleground_images/monster.png"
	};

	Sprite sky        (&shader, loadTexture(textures[0]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite graves     (&shader, loadTexture(textures[1]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite back_trees (&shader, loadTexture(textures[2]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite crypt      (&shader, loadTexture(textures[3]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite wall       (&shader, loadTexture(textures[4]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite ground     (&shader, loadTexture(textures[5]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite tree       (&shader, loadTexture(textures[6]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
 	Sprite bones      (&shader, loadTexture(textures[7]), glm::vec3(400.0, 300.0, 0.0), glm::vec3(800, 500, 1.0), 0.0);
	Sprite monster    (&shader, loadTexture(textures[8]), glm::vec3(400.0, 200.0, 0.0), glm::vec3(100, 100, 1.0), 0.0);

	//Ativando o buffer de textura 0 da opengl
	glActiveTexture(GL_TEXTURE0);
	shader.Use();
	shader.setInt("texBuffer", 0);

	//Matriz de proje��o paralela ortogr�fica
	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	shader.setMat4("projection", glm::value_ptr(projection));

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); //unidades de tela: pixel

		// Limpa o buffer de cor
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Desenho das texturas
		sky.drawShader();
		graves.drawShader();
		back_trees.drawShader();
		crypt.drawShader();
		wall.drawShader();
		ground.drawShader();
		tree.drawShader();
		bones.drawShader();
		monster.drawShader();

		// Troca os buffers da tela
		glfwSwapBuffers(window);

		// Verifica se houveram eventos de input e chama as fun��es de callback
		glfwPollEvents();
	}

	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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