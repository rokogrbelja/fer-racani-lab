#include "Renderer.h"

void Renderer::addObject(Object * object) {
	objects.push_back(object);
}

void Renderer::renderObjects() {

	glUseProgram(objects[0]->shader->ID);

	// postavi vrijednost matrice modela u shaderu
	unsigned int modelLoc = glGetUniformLocation(objects[0]->shader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(objects[0]->getModelMatrix()));

	// postavi vrijednost matrice pogleda u shaderu
	unsigned int viewLoc = glGetUniformLocation(objects[0]->shader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	// postavi vrijednost matrice projekcije u shaderu
	unsigned int projLoc = glGetUniformLocation(objects[0]->shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getPerspectiveMatrix()));

	// postavi vrijednost boje u shaderu
	unsigned int colorLoc = glGetUniformLocation(objects[0]->shader->ID, "u_color");
	glUniform3f(colorLoc, 0.2, 0.7, 0.0);


	// iscrtaj objekt
	objects[0]->mesh.drawFaces();

}

void Renderer::renderCurve(int segment, float t) {
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getPerspectiveMatrix();

	glUseProgram(curve->shader->ID);

	unsigned int viewLoc = glGetUniformLocation(curve->shader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projLoc = glGetUniformLocation(curve->shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int colorLoc = glGetUniformLocation(curve->shader->ID, "u_color");
	glUniform3f(colorLoc, 0.95, 0.1, 0.35);
	//curve->drawControlPolygon();

	colorLoc = glGetUniformLocation(curve->shader->ID, "u_color");
	glUniform3f(colorLoc, 0.15, 0.85, 0.95);
	curve->drawCurve();

	colorLoc = glGetUniformLocation(curve->shader->ID, "u_color");
	glUniform3f(colorLoc, 0.9, 0.9, 0.0);
	curve->drawTangent(segment, t);

	colorLoc = glGetUniformLocation(curve->shader->ID, "u_color");
	glUniform3f(colorLoc, 0.9, 0.9, 5.0);
	curve->drawNormal(segment, t);
}

