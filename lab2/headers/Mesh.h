#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Vertex {
	glm::vec3 Position;
};

class Mesh {

private:

	unsigned int VBO, EBO;

	void setupMesh();

public:

	unsigned int VAO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	Mesh();

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);

	void updateMesh();

	vector<Vertex> getBoundingBox();

	void applyTransform(glm::mat4 transformationMatrix);

	void drawFaces();

	void drawWireframe();

};