#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

char processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		return 'r';
	}
	return 0;
}

const char* vertextShaderSource = 
"#version 330 core\n"  //using 3.3 version
"layout (location = 0) in vec3 position;\n"   // define the input vertex attr with key word 'in'
"void main(){\n"
"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n" //convert the input vertex data into the shader out data
"}\0";

const char* fragnmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glifwCreateWindow(int width, int height, string name, pointer monitor, pointer share)
	GLFWwindow* window = glfwCreateWindow(800, 800, "openglProject1", NULL, NULL);
	if (window == NULL) {
		cout << "failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// here, we create a object which will be preseasent in screen

	// initialize glad
	// glfw gives the corrent function based on OS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "failed to initialize GLAD" << endl;
		return -1;
	}

	// ? why there is a port, 
	glViewport(0, 0, 800, 800);

	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// init some vertex
	// vertices data, contains three vertex
	float vertices[] = {
		-0.5f, -0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.35f,0.0f,
		1.0f,0.35f,0.0f
	};


	// define vertex shader
	unsigned int vex_shader;
	vex_shader = glCreateShader(GL_VERTEX_SHADER); //create the shader and specify with vertex shader
	glShaderSource(vex_shader, 1, &vertextShaderSource, NULL); //bind the source code to this shader
	glCompileShader(vex_shader); //compile the shader
	int vex_success;
	char vex_infoLog[512];
	glGetShaderiv(vex_shader, GL_COMPILE_STATUS, &vex_success);
	if (!vex_success) {
		glGetShaderInfoLog(vex_shader, 512, NULL, vex_infoLog);
		cout << "vertex shader compile failed:" << vex_infoLog << endl;
	}

	//define fragment shader
	unsigned int frg_shader;
	frg_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frg_shader, 1, &fragnmentShaderSource, NULL);
	glCompileShader(frg_shader);
	int frg_success;
	char frg_infoLog[512];
	glGetShaderiv(frg_shader, GL_COMPILE_STATUS, &frg_success);
	if (!frg_success) {
		glGetShaderInfoLog(frg_shader, 512, NULL, frg_infoLog);
		cout << "fragment shader compile failed:" << frg_infoLog << endl;
	}

	//link these shader into a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vex_shader);
	glAttachShader(shaderProgram, frg_shader);
	glLinkProgram(shaderProgram);

	
	//delete the fomer shader, once it is linked, it's useless
	glDeleteShader(vex_shader);
	glDeleteShader(frg_shader);



	//define VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// bind VAO
	glBindVertexArray(VAO);
	// copy the  vertices data in a buffer 
	// can be lots of object
	// define VBO
	unsigned int tri_VBO;
	glGenBuffers(1, &tri_VBO); //bind a index for the buffer
	glBindBuffer(GL_ARRAY_BUFFER, tri_VBO); //indicate the type of this buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy the vertex data into the buffer's memory
	//define EBO
	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// interpret the vertex data into vertex attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //enable the 0 index attr
	//activate the program use the shader
	glUseProgram(shaderProgram);
	// unbind VAO
	glBindVertexArray(0);




	// create a render loop
	while (!glfwWindowShouldClose(window)) {

		// get input
		char result = processInput(window);

		//render
		// ...
		if (result == 'r') {
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		//check events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}

	glfwTerminate();
	return 0;
}