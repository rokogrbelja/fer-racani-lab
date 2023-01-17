#pragma once

#include "Transform.h"

#include <iostream>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {

private:
	// relevantni vektori kamere
	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUp;

	// eulerovi kutovi
	float Yaw;
	float Pitch;

	void updateCameraVectors();

public:
	float MouseSensitivity;
	float MovementSpeed;
	float Zoom;

	Camera();

	glm::mat4 getViewMatrix();

	glm::mat4 getPerspectiveMatrix();

	void ProcessMouseMovement(float xoffset, float yoffset);

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void ProcessMouseScroll(float yoffset);
};