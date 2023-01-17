#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices) {

	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

void Mesh::setupMesh() {
	// generate array
	glGenVertexArrays(1, &VAO);
	//generate vertex buffer
	glGenBuffers(1, &VBO);
	//generate element buffer
	glGenBuffers(1, &EBO);

	//bind array
	glBindVertexArray(VAO);

		//load vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//load element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//describe vertex structure data
		//vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		//as we add new attributes to the Vertex struct, we are going to set their attribute pointers
		// ...

	//unbind array
	glBindVertexArray(0);

}

void Mesh::updateMesh() {
	//update vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

vector<Vertex> Mesh::getBoundingBox() {

	float xmin, xmax, ymin, ymax, zmin, zmax;
	Vertex vmin, vmax;
	vector<Vertex> boundingBox;

	xmin = xmax = vertices[0].Position.x;
	ymin = ymax = vertices[0].Position.y;
	zmin = zmax = vertices[0].Position.z;

	for (int i = 1; i < vertices.size(); i++) {
		xmin = min(vertices[i].Position.x, xmin);
		ymin = min(vertices[i].Position.y, ymin);
		zmin = min(vertices[i].Position.z, zmin);
		xmax = max(vertices[i].Position.x, xmax);
		ymax = max(vertices[i].Position.y, ymax);
		zmax = max(vertices[i].Position.z, zmax);
	}

	vmin.Position.x = xmin;
	vmin.Position.y = ymin;
	vmin.Position.z = zmin;
	vmax.Position.x = xmax;
	vmax.Position.y = ymax;
	vmax.Position.z = zmax;

	boundingBox.push_back(vmin);
	boundingBox.push_back(vmax);

	return boundingBox;
}

void Mesh::applyTransform(glm::mat4 transformationMatrix) {
	for (int i = 0; i < vertices.size(); i++) {
		//get vertex position in the form of a homogeneous vector with the homogeneous coordinate set to 1
		glm::vec4 vertex = glm::vec4(
			vertices[i].Position.x,
			vertices[i].Position.y,
			vertices[i].Position.z,
			1.0f);
		//calculates the transformation
		vertex = transformationMatrix * vertex;
		//get homogeneous coordinate
		float h = vertex[3];
		//remove homogeneous coordinate
		glm::vec3 newVertex = (1/h) * glm::vec3(vertex[0], vertex[1], vertex[2]);
	
		//set new vertex positions in mesh
		vertices[i].Position.x = newVertex.x;
		vertices[i].Position.y = newVertex.y;
		vertices[i].Position.z = newVertex.z;
	}
	updateMesh();
}

void Mesh::drawFaces() {
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::drawWireframe() {
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
