#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"
#include"stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<math.h>

// force to using amd graphics card
extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

using namespace std;

const int windowWidth = 800;
const int windowHeight = 800;

bool keys[1024];
// calculate render time cost
float deltaTime = 0.0f; // the render time cost = current frame rendered timestamp - lastFrame
float lastFrame = 0.0f; //the timestamp of rendered last frame

void resize_window(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
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
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowHeight, windowHeight);

	glfwSetFramebufferSizeCallback(mainWindow, resize_window);

	glfwSetKeyCallback(mainWindow, keyCallback);



	

	float vertices[] = {
		// ---position--		--texture--
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};

	int elementIndex[] = {
		// --draw squence--
		0, 1, 3,
		1, 2, 3
	};


	// input texture
	int width, height, nrChannels;
	unsigned char* textureImage = stbi_load("chen.jpg", &width, &height, &nrChannels, 0); //using stb lib to get image byte stream 

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage); // full the data into the texture object
	glGenerateMipmap(GL_TEXTURE_2D); //indiate the mipmaps,  this procecss is necessary
	stbi_image_free(textureImage); //free the memory space
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

	float gree_axis_value = 1.0f;

	// define look at matrix parm
	glm::vec3 position_camera = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 relative_target = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);


	while (!glfwWindowShouldClose(mainWindow)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		
		float cameraSpeed = 5.0f * deltaTime;
		if (keys[GLFW_KEY_W]) {
			position_camera += glm::normalize(relative_target) * cameraSpeed;
		}
		if (keys[GLFW_KEY_S]) {
			position_camera -= glm::normalize(relative_target) * cameraSpeed;
		}
		if (keys[GLFW_KEY_A]) {
			position_camera += glm::normalize(glm::cross(world_up, relative_target)) * cameraSpeed;
		}
		if (keys[GLFW_KEY_D]) {
			position_camera += glm::normalize(glm::cross(relative_target, world_up)) * cameraSpeed;
		}

		// define matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, 0.6f, glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(position_camera, position_camera + relative_target, world_up);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);


		unsigned int modelMat = glGetUniformLocation(shader.ID, "modelMat");
		unsigned int viewMat = glGetUniformLocation(shader.ID, "viewMat");
		unsigned int projectionMat = glGetUniformLocation(shader.ID, "projectionMat");
		shader.use();
		glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(baseVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//calculate the frame render cost
		float currentTimeStamp = glfwGetTime();
		deltaTime = currentTimeStamp - lastFrame;
		lastFrame = currentTimeStamp;

		//check events and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(mainWindow);
		
	}


	glfwTerminate();
	return 0;

}