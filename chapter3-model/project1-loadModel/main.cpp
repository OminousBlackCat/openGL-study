#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"
#include"stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<math.h>
#include"camera.h"
#include <random>
#include <time.h>
#include"model.h"

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

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

float viewFov = 45.0f;

Camera myCamera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));

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


void scrollCallbak(GLFWwindow* window, double xoffset, double yoffset) {
	myCamera.zoom(yoffset);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	myCamera.yawAndPitch(xoffset, yoffset);
}


int main() {
	srand(time(NULL));

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

	glfwSetCursorPosCallback(mainWindow, mouseCallback);

	glfwSetScrollCallback(mainWindow, scrollCallbak);

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);





	// 定义第一个模型
	Model spider("obj/Only_Spider_with_Animations_Export.obj");




	Shader shader1("vertexShader.vs", "fragmentShader.fs");



	while (!glfwWindowShouldClose(mainWindow)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		
		//process keyboard events
		float cameraSpeed = 5.0f * deltaTime;
		if (keys[GLFW_KEY_W]) {
			myCamera.movement(Camera::CAMERA_FORWARD, deltaTime);
		}
		if (keys[GLFW_KEY_S]) {
			myCamera.movement(Camera::CAMERA_BACKWARD, deltaTime);
		}
		if (keys[GLFW_KEY_A]) {
			myCamera.movement(Camera::CAMERA_LEFT, deltaTime);
		}
		if (keys[GLFW_KEY_D]) {
			myCamera.movement(Camera::CAMERA_RIGHT, deltaTime);
		}

		

		// define matrix
		glm::mat4 model = glm::mat4(1.0f);
		

		glm::mat4 view = glm::mat4(1.0f);
		view = myCamera.getLookAtMat();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(myCamera.zoomAngle, (float)windowWidth / (float)windowHeight, 1.0f, 100.0f);

		//使用物体的着色器
		shader1.use();
		unsigned int modelMat = glGetUniformLocation(shader1.ID, "modelMat");
		unsigned int viewMat = glGetUniformLocation(shader1.ID, "viewMat");
		unsigned int projectionMat = glGetUniformLocation(shader1.ID, "projectionMat");
		unsigned int viewPos = glGetUniformLocation(shader1.ID, "viewPos");


		//设置光源属性
		unsigned int lightAmbient = glGetUniformLocation(shader1.ID, "flashLight.ambient");
		unsigned int lightDiffuse = glGetUniformLocation(shader1.ID, "flashLight.diffuse");
		unsigned int lightSpecular = glGetUniformLocation(shader1.ID, "flashLight.specular");
		unsigned int lightPosition = glGetUniformLocation(shader1.ID, "flashLight.position");
		unsigned int lightDirection = glGetUniformLocation(shader1.ID, "flashLight.direction");
		unsigned int flashCutOff = glGetUniformLocation(shader1.ID, "flashLight.cutOff");
		unsigned int flashOutCutOff = glGetUniformLocation(shader1.ID, "flashLight.outerCutOff");
		unsigned int constant = glGetUniformLocation(shader1.ID, "flashLight.attenuation_constant");
		unsigned int liner = glGetUniformLocation(shader1.ID, "flashLight.attenuation_linear");
		unsigned int quadratic = glGetUniformLocation(shader1.ID, "flashLight.attenuation_quadratic");
		glUniform3f(lightAmbient, 0.1f, 0.1f, 0.1f);
		glUniform3f(lightDiffuse, 0.7f, 0.7f, 0.7f);
		glUniform3f(lightSpecular, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosition, myCamera.position.x, myCamera.position.y, myCamera.position.z);
		glUniform3f(lightDirection, myCamera.frontVec.x, myCamera.frontVec.y, myCamera.frontVec.z);
		glUniform1f(flashCutOff, 0.98f);
		glUniform1f(flashOutCutOff, 0.94f);
		glUniform1f(constant, 1.0f);
		glUniform1f(liner, 0.14f);
		glUniform1f(quadratic, 0.07f);

		glUniform3f(viewPos, myCamera.position.x, myCamera.position.y, myCamera.position.z);
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(projection));

		
		spider.Draw(shader1);
			

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