#include <iostream>
#include <string>
#include <assert.h>

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB IMAGE
#include<stb_image.h>

using namespace std;

//Classe para manipula��o dos shaders
#include "Shader.h"
#include <vector>

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();
std::vector<GLuint> loadTextures(int VAOsLength);
void drawScene(unsigned int* VAOs, std::vector<GLuint> texIDs, int texturesQty);

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

	// GLAD: carrega todos os ponteiros das fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	// Informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/tex_vert.glsl", "../shaders/tex_frag.glsl");

	// Cria��o das VAOs para cada uma das 8 imagens
	unsigned int VAOs[8];
	int texturesQty = sizeof(VAOs) / sizeof(VAOs[0]);
	
	// Cria��o da geometria de cada VAO
	for (GLuint i = 0; i < texturesQty; i++) {
		VAOs[i] = setupGeometry();
	}

	// Cria��o dos par�metros e IDs das texturas
	std::vector<GLuint> texIDs = loadTextures(texturesQty);

	//Matriz de proje��o paralela ortogr�fica
	glm::mat4 projection = glm::ortho(0.0,800.0,0.0,600.0,-1.0,1.0);
	//Enviando para o shader a matriz como uma var uniform
	shader.setMat4("projection", glm::value_ptr(projection));

	//Matriz da imagem de fundo (matriz de modelo)
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(800.0, 500.0, 0.0));
	shader.setMat4("model", glm::value_ptr(model));

	shader.Use();
	shader.setInt("texBuffer", 0);

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); //unidades de tela: pixel
		
		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Chamadas de desenho da cena
		drawScene(VAOs, texIDs, texturesQty);

		// Troca os buffers da tela
		glfwSwapBuffers(window);

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();
	}
	// Desaloca os buffers
	for (unsigned int i = 0; i < texturesQty; i++) {
		glDeleteVertexArrays(1, &VAOs[i]);
	}
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser armazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {
		//x     y    z    r    g    b    s    t
		//Triangulo 0
		-0.5f , 0.5f, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,  //v0
		-0.5f ,-0.5f, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,  //v1
		 0.5f , 0.5f, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,  //v2
		 //Triangulo 1	
	    -0.5f ,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //v1
		 0.5f ,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //v3
		 0.5f , 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f   //v2
	};

	GLuint VBO, VAO;
	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Atributo 0 - posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo 2 - coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

void drawScene(unsigned int* VAOs, std::vector<GLuint> texIDs, int texturesQty)
{
	for (int i = 0; i < texturesQty; i++) {
		glBindTexture(GL_TEXTURE_2D, texIDs[i]);
		glBindVertexArray(VAOs[i]); // Conectando ao buffer de geometria
		glDrawArrays(GL_TRIANGLES, 0, 6); // Para os tri�ngulos

		glActiveTexture(GL_TEXTURE0);

	}

	glBindTexture(GL_TEXTURE_2D, 0); //unbind
	glBindVertexArray(0); //Desconectando o buffer de geometria
}

std::vector<GLuint> loadTextures(int texturesQty)
{
	// Armazenamento do diret�rio de cada textura
	std::string textures[] = {
		"../battleground_images/sky.png",
		"../battleground_images/graves.png",
		"../battleground_images/back_trees.png",
		"../battleground_images/crypt.png",
		"../battleground_images/wall.png",
		"../battleground_images/ground.png",
		"../battleground_images/tree.png",
		"../battleground_images/details.png"
	};

	// Cria��o do vetor que ir� armazenar o ID de cada textura
	std::vector<GLuint> texIDs(texturesQty);

	for (GLuint i = 0; i < texturesQty; i++) {
		// Gera o identificador da textura na mem�ria 
		glGenTextures(1, &texIDs[i]);
		glBindTexture(GL_TEXTURE_2D, texIDs[i]);

		//Configura��o do par�metro WRAPPING nas coords s e t
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Configura��o do par�metro FILTERING na minifica��o e magnifica��o da textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true); // comando para que a imagem n�o fique invertida
		unsigned char* data = stbi_load(textures[i].c_str(), &width, &height, &nrChannels, 0);

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
	}

	return texIDs;
}
