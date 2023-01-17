#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Camera.h"
#include "Transform.h"
#include "BSplineCurve.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Renderer {

private:
	std::vector<Object*> objects;

public:

	Camera * camera;

	BSplineCurve * curve;

	void addObject(Object * object);

	void renderObjects();
	void renderCurve(int segment, float t);
};