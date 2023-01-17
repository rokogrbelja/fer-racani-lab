#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>

class Object : public Transform {

private:

	void loadModelFromFile(string const path);

	vector<Vertex> loadVertices(aiMesh * mesh);

	vector<unsigned int> loadIndices(aiMesh * mesh);

	void normalizeObject();

public:

	Mesh mesh;
	Shader * shader;

	Object();

	Object(string const path);

};