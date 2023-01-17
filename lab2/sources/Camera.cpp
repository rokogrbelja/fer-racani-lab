#include "Camera.h"

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	cameraFront = glm::normalize(front);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

Camera::Camera() {
	cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 10.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	MouseSensitivity = SENSITIVITY;
	MovementSpeed = SPEED;
	Zoom = ZOOM;
	updateCameraVectors();
}

glm::mat4 Camera::getPerspectiveMatrix() {
	glm::mat4 projection = Transform::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
	return projection;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	switch (direction) {
	case FORWARD:
		cameraPosition += cameraFront * velocity;
		break;
	case BACKWARD:
		cameraPosition -= cameraFront * velocity;
		break;
	case LEFT:
		cameraPosition -= cameraRight * velocity;
		break;
	case RIGHT:
		cameraPosition += cameraRight * velocity;
		break;
	case UP:
		cameraPosition += cameraUp * velocity;
		break;
	case DOWN:
		cameraPosition -= cameraUp * velocity;
		break;
}

}

void Camera::ProcessMouseScroll(float yoffset) {
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}
