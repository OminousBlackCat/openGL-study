#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"

using namespace std;

const int windowWidth = 800;
const int windowHeight = 800;

void resize_window(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

char processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		return 'a';
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		return 'd';
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		return 'w';
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		return 's';
	}
	return 0;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow = glfwCreateWindow(windowWidth, windowHeight, "opengl-project2", NULL, NULL);
	if (mainWindow == NULL) {
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to initialize GLAD" << endl;
		return -1;
	}
	
	glViewport(0, 0, windowHeight, windowHeight);

	glfwSetFramebufferSizeCallback(mainWindow, resize_window);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.35f,0.0f
	};

	// define VAO
	unsigned int baseVAO;
	glGenVertexArrays(1, &baseVAO);
	glBindVertexArray(baseVAO);

	unsigned int tri_vertex;
	glGenBuffers(1, &tri_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, tri_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	Shader shader("vertexShader.vs", "fragmentShader.fs");
	shader.use();
	glBindVertexArray(0);

	float x_axis_offset = 0.0f;
	float gree_axis_value = 1.0f;

	while (!glfwWindowShouldClose(mainWindow)) {
		char result = processInput(mainWindow);

		switch (result)
		{
		case 'a':
			x_axis_offset -= 0.00005f;
			break;
		case 'd':
			x_axis_offset += 0.00005f;
			break;
		case 'w':
			gree_axis_value += 0.0001f;
			break;
		case 's':
			gree_axis_value -= 0.0001f;
			break;
		default:
			break;
		}

		unsigned int x_axis_uniform = glGetUniformLocation(shader.ID, "x_axis_offset");
		unsigned int red_axis_uniform = glGetUniformLocation(shader.ID, "currentColor");
		shader.use();
		glUniform1f(x_axis_uniform, x_axis_offset);
		glUniform4f(red_axis_uniform, 1.0f, gree_axis_value, 0.0f, 0.0f);
		glBindVertexArray(baseVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);



		//check events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(mainWindow);
	}


	glfwTerminate();
	return 0;

}