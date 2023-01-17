#include "Transform.h"

void Transform::setOriginalPosition(glm::vec3 position) { this->originalPosition = position; }
void Transform::setOriginalOrientation(glm::mat4 orientation) { this->originalOrientation = orientation; }
void Transform::setViewUp(glm::vec3 viewUpVector) { this->viewUp = viewUpVector; }
void Transform::setSize(float size) { this->size = size; }

glm::vec3 Transform::getOriginalPosition() { return originalPosition; }
glm::mat4 Transform::getOriginalOrientation() { return originalOrientation; }
glm::vec3 Transform::getViewUp() { return viewUp; }
float Transform::getSize() { return size; }

Transform::Transform() {
	originalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	originalOrientation = glm::mat4(1.0f);
	viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
	size = 1.0f;

	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);
}

void Transform::updatePosition(glm::vec3 position) {
	translationMatrix = Transform::translate3D(position);
}

void Transform::updateOrientation(glm::vec3 xaxis, glm::vec3 yaxis, glm::vec3 zaxis) {
	rotationMatrix = coorSystemRotation3D(xaxis, yaxis, zaxis);
}

glm::mat4 Transform::translate3D(glm::vec3 translateVector) {
	// transformation matrix that translates in direction of the given vector
	glm::mat4 trans = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translateVector.x, translateVector.y, translateVector.z, 1.0f);
	return trans;
}

glm::mat4 Transform::rotate3D(float angle, glm::vec3 axis) {
	// transformation matrix that rotates by given angle around given axis
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle), axis);
	return trans;
}

glm::mat4 Transform::coorSystemRotation3D(glm::vec3 w, glm::vec3 u, glm::vec3 v) {
	// transformation matrix that rotates an object so that its local coordinate axes
	// match the given coordinate axes
	glm::mat4 trans = glm::mat4(
		w.x, w.y, w.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		v.x, v.y, v.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return trans;
}

glm::mat4 Transform::scale3D(glm::vec3 scaleVector) {
	//transformation matrix that scales coresponding axes by given factors
	glm::mat4 trans = glm::mat4(
		scaleVector.x, 0.0f, 0.0f, 0.0f,
		0.0f, scaleVector.y, 0.0f, 0.0f,
		0.0f, 0.0f, scaleVector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return trans;
}

glm::mat4 Transform::lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp) {

	//cameraDirection (N)
	glm::vec3 zaxis = glm::normalize(eye - center);
	//camera right vector (U) - cross product of camera direction and viewUp vector
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(viewUp), zaxis));
	//camera up vector (V)
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	//create translation matrix
	glm::mat4 translation = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-eye.x, -eye.y, -eye.z, 1.0f);

	//create rotation matrix
	glm::mat4 rotation = glm::mat4(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return rotation * translation;
}

glm::mat4 Transform::frustum(float l, float r, float b, float t, float n, float f) {
	glm::mat4 projection = glm::mat4(
		(2 * n) / (r - l), 0.0f, 0.0f, 0.0f,
		0.0f, (2 * n) / (t - b), 0.0f, 0.0f,
		(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1.0f,
		0.0f, 0.0f, -(2 * f*n) / (f - n), 0.0f);

	return projection;
}

void Transform::addTransformation(glm::mat4 trans) {
	transformations.push_back(trans);
}

void Transform::move(glm::vec3 direction) {
	glm::mat4 trans = translate3D(direction);
	addTransformation(trans);
}

void Transform::rotate(float angle, glm::vec3 axis) {
	glm::mat4 trans = rotate3D(angle, axis);
	addTransformation(trans);
}

void Transform::scale(glm::vec3 factors) {
	glm::mat4 trans = scale3D(factors);
	addTransformation(trans);
}

glm::mat4 Transform::getModelMatrix() {
	//initialize identity matrix
	glm::mat4 model = glm::mat4(1.0f);

	model = translationMatrix * glm::inverse(glm::transpose(rotationMatrix)) * originalOrientation * model;

	//multiply all accumulated transformation matrices
	/*for (int i = 0; i < transformations.size(); i++) {
		model = transformations[i] * model;
	}*/
	return model;
}

