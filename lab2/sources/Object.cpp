#include "Object.h"

void Object::loadModelFromFile(string const path) {

	//read object file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, 
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene) {
		std::cerr << importer.GetErrorString();
		return;
	}

	//process the first mesh (assume there is only a singular mesh described in the file)
	aiMesh * mesh = scene->mMeshes[0];
	vector<Vertex> vertices = loadVertices(mesh);
	vector<unsigned int> indices = loadIndices(mesh);

	this->mesh = Mesh(vertices, indices);

	//apply transformations necessary to normalize the object
	normalizeObject();
}

vector<Vertex> Object::loadVertices(aiMesh * mesh) {

	vector<Vertex> vertices;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.Position = vector;
		vertices.push_back(vertex);
	}

	return vertices;
}

vector<unsigned int> Object::loadIndices(aiMesh * mesh) {

	vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return indices;
}

void Object::normalizeObject() {

	vector<Vertex> boundingBox = mesh.getBoundingBox();

	//set minimum and maximum coordinates
	float xmin = boundingBox[0].Position.x;
	float ymin = boundingBox[0].Position.y;
	float zmin = boundingBox[0].Position.z;
	float xmax = boundingBox[1].Position.x;
	float ymax = boundingBox[1].Position.y;
	float zmax = boundingBox[1].Position.z;

	//center coordinates
	float xc = (xmin + xmax) / 2;
	float yc = (ymin + ymax) / 2;
	float zc = (zmin + zmax) / 2;

	//maximum range
	float M = std::max({ xmax - xmin, ymax - ymin, zmax - zmin });

	//translate all vertices in direction of vector (-xc, -yc, -zc)
	mesh.applyTransform(translate3D(glm::vec3(-xc, -yc, -zc)));

	//scale all vertices 2/M
	mesh.applyTransform(scale3D((2.0f/M) * glm::vec3(1.0f, 1.0f, 1.0f)));

}

Object::Object() {
}

Object::Object(string const path) : Transform() {
	loadModelFromFile(path);
}
