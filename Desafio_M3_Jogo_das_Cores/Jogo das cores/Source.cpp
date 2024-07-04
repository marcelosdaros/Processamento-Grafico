/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * Desenvolvido por Marcelo Strack Daros para a disciplina de Processamento Gráfico - Unisinos
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib> 
#include <ctime>   

using namespace std;

//Classe para manipulação dos shaders
#include "Shader.h"
#include <vector>
#include <iostream>

// Protótipo da função de callback da GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Protótipos das funções
int setup();
glm::vec3 randomColor();
glm::vec3* generateRandomColors(int columns, int lines);
glm::vec3 pickColor(GLdouble xpos, GLdouble ypos);
float calculateEuclideanDistance(glm::vec3 color1, glm::vec3 color2);
bool colorsMatch(glm::vec3 color1, glm::vec3 color2);
glm::vec3 getRGBcolor(float red, float green, float blue);

// Dimensões da janela
const GLuint WIDTH = 1024, HEIGHT = 768;

const int COLUMNS = 5, LINES = 11;
int rounds = 8;
int score = 0;
glm::vec3 colorSelected = glm::vec3(0.0f, 0.0f, 0.0f);

int main()
{
	cout << "Bem vindo ao Jogo das Cores!" << endl;
	cout << "55 retangulos serao exibidos na tela. Clique em um retangulo para eliminar todos os que possuirem uma cor similar, tente eliminar sempre o maior numero de retangulos possivel em um unico clique!" << endl;
	cout << "Voce possui 8 tentativas e cada retangulo eliminado adiciona 1 ponto na pontuacao total, portanto maior pontuacao possivel: 55." << endl;
	cout << "Boa sorte!" << endl;

	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo das cores", nullptr, nullptr);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o shader
	Shader shader("../shaders/retangulo.vs", "../shaders/retangulo.fs");
	Shader shaderPalete("../shaders/palete.vs", "../shaders/palete.fs");

	// Gerando um buffer simples
	GLuint VAO = setup();
	glm::mat4 model = glm::mat4(1);

	// Array para armazenar as cores sorteadas e usar na criação de cada retângulo
	glm::vec3* colors = generateRandomColors(COLUMNS, LINES);

	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO); // Conectando ao buffer de geometria

		shader.Use();
		GLfloat xc = -0.77f,
			    xl = 0.90f;
		int colorIndex = 0; // Variável de controle do índice da cor a ser usada (0 a 54)

		for (int c = 0; c < COLUMNS; c++) 
			for (int l = 0; l < LINES; l++) {

				// Se a cor selecionada no clique é diferente de preto, verifica se cada retângulo tem cor similar à cor do clique
				if (colorSelected != glm::vec3(0.0f, 0.0f, 0.0f)) {
					glm::vec3 rgbColor = getRGBcolor(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
					if (colorsMatch(colorSelected, rgbColor)) {
						colors[colorIndex] = glm::vec3(0.0f, 0.0f, 0.0f);
						score += 1;
					}
				}

				shader.setVec3("cor", colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);

				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(xc + c * 0.385, xl - l * 0.125, 0));
				model = glm::scale(model, glm::vec3(0.38, 0.38, 1));
				shader.setMat4("model", glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				colorIndex = colorIndex + 1;
			}

			// Término do jogo
			if (rounds == 0) {
				cout << "Fim de Jogo!" << endl;
				if (score == 55) {
					cout << "Parabens! Sua pontuacao foi 55, voce eliminou todos os retangulos!" << endl;
				} else {
					cout << "Nem todos os retangulos foram eliminados, mas sua pontuacao foi " << score << " (maximo de 55). Incrivel!" << endl;
				}
				break;
			}

		shaderPalete.Use();

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.70, -0.70, 0)); 
		model = glm::scale(model, glm::vec3(0.5, 0.5, 1)); 
		shaderPalete.setMat4("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

		// Desconectando o buffer de geometria
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Libera a memória reservada para as cores
	delete[] colors;
	// Desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Função de callback de clique do botão
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	GLdouble xpos, ypos;
	int width, height;
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwGetWindowSize(window, &width, &height);
		colorSelected = pickColor(xpos, height - ypos); // cor do pixel
		rounds--;
		cout << "Rounds: " << rounds << endl;
	}
}

// Função para sortear uma cor aleatória
glm::vec3 randomColor() {
	float red = static_cast<float>(rand()) / RAND_MAX;
	float green = static_cast<float>(rand()) / RAND_MAX;
	float blue = static_cast<float>(rand()) / RAND_MAX;
	return glm::vec3(red, green, blue);
}

// Função para gerar a quantidade necessária de cores aleatórias (colunas*linhas)
glm::vec3* generateRandomColors(int columns, int lines) {
	srand(static_cast<unsigned int>(time(nullptr)));
	int amount = columns * lines;
	glm::vec3* colors = new glm::vec3[amount];
	glm::vec3 chosenColor;

	// Loop para sortear as 55 cores, caso a cor preta seja sorteada o sorteio é feito novamente
	for (int i = 0; i < amount; ++i) {
		chosenColor = glm::vec3(0.0f, 0.0f, 0.0f);
		while (chosenColor == glm::vec3(0.0f, 0.0f, 0.0f)) {
			chosenColor = randomColor();
		}
		colors[i] = chosenColor;
	}
	return colors;
}

glm::vec3 pickColor(GLdouble xpos, GLdouble ypos) {
	unsigned char pixel[4];
	glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

	// Se a cor for diferente de preto
	if (pixel[0] || pixel[1] || pixel[2]) {
		return glm::vec3((int)pixel[0], (int)pixel[1], (int)pixel[2]);
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

// Verifica se as cores são similares
bool colorsMatch(glm::vec3 color1, glm::vec3 color2) {
	float distance = calculateEuclideanDistance(color1, color2);
	if (distance <= 100) {
		return true;
	}
	return false;
}

// Calcula a distância Euclidiana entre duas cores. Fórmula: sqrt((R2 - R1)^2 + (G2 - G1)^2 + (B2 - B1)^2)
float calculateEuclideanDistance(glm::vec3 color1, glm::vec3 color2) {
	float distance = sqrt(pow(color2.r - color1.r, 2) + pow(color2.g - color1.g, 2) + pow(color2.b - color1.b, 2));
	return distance;
}

// Retorna a cor em formato RGB de 0 a 255
glm::vec3 getRGBcolor(float red, float green, float blue) {
	return glm::vec3(red * 255.0f, green * 255.0f, blue * 255.0f);
}

// Configuração e desenho dos vertices
int setup()
{
	GLfloat vertices[] = {
		-0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vértice retângulo
		-0.5f,  0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vértice retângulo
		 0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vértice retângulo
		 0.5f,  0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vértice retângulo 
		-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,  // Vértice cores (R)
		 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  // Vértice cores (G)
		 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  // Vértice cores (B)
	    -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f   // Vértice cores (RGB)
	};

	GLuint VBO, VAO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}