#include "Animate.h"

Animate::Animate() {
}

Animate::Animate(Object * object, BSplineCurve * curve) {
	this->object = object;
	this->curve = curve;
	segment = 1;
	t = 0.0f;
	speed = 0.0025f;
}

void Animate::nextFrame() {
	t += speed;
	if (t >= 1.0f) {
		t = 0.0f;
		segment++;
		if (segment > curve->controlVertices.size() - 3) {
			segment = 1;
		}
	}
}

void Animate::updateObject() {
	glm::vec3 newPosition = curve->getSegment(segment, t);
	glm::vec3 tangent = curve->getTangent(segment, t);
	glm::vec3 normal = curve->getNormal(segment, t);
	glm::vec3 binormal = curve->getBinormal(tangent, normal);

	object->updatePosition(newPosition);
	object->updateOrientation(tangent, normal, binormal);
}
