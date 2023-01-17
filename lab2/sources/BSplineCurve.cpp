#include "BSplineCurve.h"

void BSplineCurve::calcCurveVertices(int divs) {
	glm::vec3 v;
	int n = controlVertices.size();
	
	 /* n kontrolnih tocaka odreduje n-3 segmenta krivulje
	 npr. za n = 5, i = 1 do 3, a segmenti su:
	 r0, r1, r2, r3 --- p1
	 r1, r2, r3, r4 --- p2
	 r2, r3, r4, r5 --- p3 */
	for (int i = 1; i <= n - 3; i++) {
		for (int j = 0; j < divs; j++) {
			float t = 1.0f / divs * j;

			// izracunaj tocku krivulje za zadani segment pi i parametar t
			v = getSegment(i, t);
			curveVertices.push_back(v);
		}
	}

	printf("CALCULATED CURVE VERTICES");

}

void BSplineCurve::setupCurve() {

	// generiraj VAO i VBO za vrhove kontrolnog poligona, krivulju i tangentu
	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);

	// setup control polygon buffers and arrays
	glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, controlVertices.size() * sizeof(glm::vec3), &controlVertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// setup curve buffers and arrays
	glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, curveVertices.size() * sizeof(glm::vec3), &curveVertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	// setup tangent buffers and arrays
	glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
}

BSplineCurve::BSplineCurve() {
}


BSplineCurve::BSplineCurve(std::string const path, int divs) {
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

	for (int i = 0; i < mesh->mNumVertices; i++) {
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		controlVertices.push_back(vector);
	}

	std::cout << "INDICES: ";
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			std::cout << face.mIndices[j] << " ";
			indices.push_back(face.mIndices[j]);
		}
	}

	calcCurveVertices(divs);
	setupCurve();
}

/*
Prilikom definiranja matrica za računanje tocaka i derivacija krivulje,
koristimo transponirane matrice jer glm koristi COLUMN-MAJOR pristup indeksaciji
matrica i vektora, tako da moramo transponirati kako bi mogli očuvati poredak
množenja vektora i matrica.
*/

glm::vec3 BSplineCurve::getSegment(int i, float t) {
	glm::vec4 T = glm::vec4(pow(t, 3), pow(t, 2), t, 1.0f);

	glm::mat4 B = (1.0f / 6.0f) * glm::mat4(-1.0f, 3.0f, -3.0f, 1.0f,
											3.0f, -6.0f, 0.0f, 4.0f,
											-3.0f, 3.0f, 3.0f, 1.0f,
											1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 r0 = controlVertices[i - 1];
	glm::vec3 r1 = controlVertices[i];
	glm::vec3 r2 = controlVertices[i + 1];
	glm::vec3 r3 = controlVertices[i + 2];

	glm::mat3x4 R = glm::mat3x4(r0.x, r1.x, r2.x, r3.x,
								r0.y, r1.y, r2.y, r3.y,
								r0.z, r1.z, r2.z, r3.z);

	return T * B * R;
}

glm::vec3 BSplineCurve::getTangent(int i, float t) {

	glm::vec4 T = glm::vec4(3 * pow(t, 2), 2 * t, 1.0f, 0.0f);

	glm::mat4 B = (1.0f / 6.0f) * glm::mat4(-1.0f, 3.0f, -3.0f, 1.0f,
											3.0f, -6.0f, 0.0f, 4.0f,
											-3.0f, 3.0f, 3.0f, 1.0f,
											1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 r0 = controlVertices[i - 1];
	glm::vec3 r1 = controlVertices[i];
	glm::vec3 r2 = controlVertices[i + 1];
	glm::vec3 r3 = controlVertices[i + 2];

	glm::mat3x4 R = glm::mat3x4(r0.x, r1.x, r2.x, r3.x,
								r0.y, r1.y, r2.y, r3.y,
								r0.z, r1.z, r2.z, r3.z);

	return glm::normalize(T * B * R);
}

glm::vec3 BSplineCurve::getNormal(int i, float t) {
	// prva derivacija - tangenta
	glm::vec3 p1 = getTangent(i, t);

	glm::vec4 T = glm::vec4(6 * t, 2.0f, 0.0f, 0.0f);

	glm::mat4 B = (1.0f / 6.0f) * glm::mat4(-1.0f, 3.0f, -3.0f, 1.0f,
											3.0f, -6.0f, 0.0f, 4.0f,
											-3.0f, 3.0f, 3.0f, 1.0f,
											1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 r0 = controlVertices[i - 1];
	glm::vec3 r1 = controlVertices[i];
	glm::vec3 r2 = controlVertices[i + 1];
	glm::vec3 r3 = controlVertices[i + 2];

	glm::mat3x4 R = glm::mat3x4(r0.x, r1.x, r2.x, r3.x,
								r0.y, r1.y, r2.y, r3.y,
								r0.z, r1.z, r2.z, r3.z);
	// druga derivacija
	glm::vec3 p2 = glm::normalize(T * B * R);

	// normala - vektorski produkt prve i druge derivacije
	return glm::normalize(glm::cross(p1, p2));
}

glm::vec3 BSplineCurve::getBinormal(glm::vec3 tangent, glm::vec3 normal) {
	// binormala - vektorski produkt tangente i normale
	return glm::normalize(glm::cross(tangent, normal));
}


void BSplineCurve::drawControlPolygon() {
	glBindVertexArray(VAO[0]);
		glDrawArrays(GL_LINE_STRIP, 0, controlVertices.size());
	glBindVertexArray(0);
}

void BSplineCurve::drawCurve() {
	glBindVertexArray(VAO[1]);
		glDrawArrays(GL_LINE_STRIP, 0, curveVertices.size());
	glBindVertexArray(0);
}

void BSplineCurve::drawTangent(int i, float t) {
	std::vector<glm::vec3> tan;
	glm::vec3 start = getSegment(i, t);
	glm::vec3 end = start + getTangent(i, t);
	tan.push_back(start);
	tan.push_back(end);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &tan[0], GL_STATIC_DRAW);

	glBindVertexArray(VAO[2]);
		glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(0);
	
}

void BSplineCurve::drawNormal(int i, float t) {
	std::vector<glm::vec3> tan;
	glm::vec3 start = getSegment(i, t);
	glm::vec3 end = start + getNormal(i, t);
	tan.push_back(start);
	tan.push_back(end);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &tan[0], GL_STATIC_DRAW);

	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(0);

}