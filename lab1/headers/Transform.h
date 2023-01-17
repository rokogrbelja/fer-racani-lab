#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Transform {

private:

	glm::vec3 originalPosition;
	glm::mat4 originalOrientation;
	glm::vec3 viewUp;
	float size;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;

	std::vector<glm::mat4> transformations;

public:

	void setOriginalPosition(glm::vec3);
	void setOriginalOrientation(glm::mat4 orientation);
	void setViewUp(glm::vec3 viewUpVector);
	void setSize(float size);

	glm::vec3 getOriginalPosition();
	glm::mat4 getOriginalOrientation();
	glm::vec3 getViewUp();
	float getSize();

	Transform();

	void updatePosition(glm::vec3 position);
	void updateOrientation(glm::vec3 xaxis, glm::vec3 yaxis, glm::vec3 zaxis);

	// functions that generate transformation matrices
	static glm::mat4 translate3D(glm::vec3 translateVector);
	static glm::mat4 rotate3D(float angle, glm::vec3 axis);
	static glm::mat4 coorSystemRotation3D(glm::vec3 w, glm::vec3 u, glm::vec3 v);
	static glm::mat4 scale3D(glm::vec3 scaleVector);
	static glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp);
	static glm::mat4 frustum(float l, float r, float b, float t, float n, float f);

	void addTransformation(glm::mat4 trans);

	// functions that accumulate transformations of the original object vertices
	void move(glm::vec3 translateVector);
	void rotate(float angle, glm::vec3 axis);
	void scale(glm::vec3 scaleVector);

	glm::mat4 getModelMatrix();

	virtual void ProcessMouseMovement(float xoffset, float yoffset, bool clicked) {}
};