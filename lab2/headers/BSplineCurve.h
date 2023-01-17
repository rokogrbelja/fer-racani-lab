#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <stdlib.h>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

class BSplineCurve {

private:
	unsigned int VBO[3];

	void calcCurveVertices(int divs);

	void setupCurve();

public:
	std::vector<glm::vec3> controlVertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> curveVertices;

	unsigned int VAO[3];

	Shader * shader;

	BSplineCurve();

	BSplineCurve(std::string const path, int divs);

	glm::vec3 getSegment(int i, float t);

	glm::vec3 getTangent(int i, float t);

	glm::vec3 getNormal(int i, float t);

	glm::vec3 getBinormal(glm::vec3 tangent, glm::vec3 normal);

	void drawControlPolygon();

	void drawCurve();

	void drawTangent(int i, float t);

	void drawNormal(int i, float t);
};