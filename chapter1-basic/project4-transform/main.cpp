#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"
#include<SOIL/SOIL.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<math.h>

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
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		return 'r';
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
		// ---position--		--texture--
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		0.5f,  -0.5f, 0.0f,		1.0f, 0.0f,
		0.5f,   0.5f, 0.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
	};

	int elementIndex[] = {
		// --draw squence--
		0, 1, 2,
		0, 2, 3
	};


	// input texture
	int width, height;
	unsigned char* textureImage = SOIL_load_image("chen.jpg", &width, &height, 0, SOIL_LOAD_RGB); //using SOIL lib to get image byte stream 

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage); // full the data into the texture object
	glGenerateMipmap(GL_TEXTURE_2D); //indiate the mipmaps,  this procecss is necessary
	SOIL_free_image_data(textureImage); //free the memory space
	glBindTexture(GL_TEXTURE_2D, 0);


	// define VAO
	unsigned int baseVAO;
	glGenVertexArrays(1, &baseVAO);
	glBindVertexArray(baseVAO);

	unsigned int tri_vertex;
	unsigned int tri_index;
	glGenBuffers(1, &tri_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, tri_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &tri_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndex), elementIndex, GL_STATIC_DRAW);
	//map the layout in vertex shader
	//--------------index, size, type, ifNormalize, stride(offset), pointer(the space between first space)-------  
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0); //enable location = 0
	glEnableVertexAttribArray(1); //enable location = 1

	Shader shader("vertexShader.vs", "fragmentShader.fs");
	shader.use();
	glBindVertexArray(0);

	float x_axis_offset = 0.0f;
	float gree_axis_value = 1.0f;


	while (!glfwWindowShouldClose(mainWindow)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		char result = processInput(mainWindow);

		glm::mat4 transMat = glm::mat4(1.0f); // construct trans matrix

		switch (result)
		{
		case 'a': // translate to right
			transMat = glm::translate(transMat, glm::vec3(-0.01f, 0.0f, 0.0f));
			break;
		case 'd': // translate to left
			transMat = glm::translate(transMat, glm::vec3(+0.01f, 0.0f, 0.0f));
			break;
		case 'w':
			gree_axis_value += 0.0001f;
			break;
		case 's':
			gree_axis_value -= 0.0001f;
			break;
		case 'r': // rotate
			transMat = glm::rotate(transMat, 0.01f, glm::vec3(0.0, 0.0, 1.0));
			break;
		default:
			break;
		}

		glm::mat4 rotateMat = glm::mat4(1.0f);
		rotateMat = glm::rotate(transMat, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 scalMat = glm::mat4(1.0f);
		float scal = sin(glfwGetTime() / 10);
		scalMat = glm::scale(scalMat, glm::vec3(scal, scal, scal));
		transMat = scalMat * rotateMat;


		
		unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
		unsigned int red_axis_uniform = glGetUniformLocation(shader.ID, "currentColor");
		shader.use();
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMat));
		glUniform4f(red_axis_uniform, 1.0f, gree_axis_value, 0.0f, 0.0f);
		glBindVertexArray(baseVAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);



		//check events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(mainWindow);
	}


	glfwTerminate();
	return 0;

}