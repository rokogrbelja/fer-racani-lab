#pragma once

#include "Object.h"
#include "BSplineCurve.h"

class Animate {
	private:
		Object * object;
		BSplineCurve * curve;

		float speed; //must be lesser than 1, ideally between 0.01 and 0.001


	public:
		int segment;
		float t;
		Animate();
		Animate(Object * object, BSplineCurve * curve);

		void nextFrame();

		void updateObject();
};