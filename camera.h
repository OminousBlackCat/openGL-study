#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// const camera initialize value
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ZOOM = 45.0f;
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
public:
	// attribute
	glm::vec3 position; // camera world positon
	glm::vec3 frontVec; // camera front Vector, negtive with direction vector
	glm::vec3 up; // camera up vector
	glm::vec3 right; // camera right vector
	// euler angles
	float yaw;
	float pitch;
	// options
	float movementSpeed; // control camera move
	float cursorSensitivity; // control cursor
	float scrollSensitivity; // control scroll
	float zoomAngle; // control zooming

	//constructor
	Camera(glm::vec3 postion)





};