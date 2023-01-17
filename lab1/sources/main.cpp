#pragma once
// Kompajliranje:
// g++ -o prozor prozor.cpp util.cpp -lGLEW -lGL -lGLU -lglut -lpthread

#ifdef _WIN32
#include <windows.h>             //bit ce ukljuceno ako se koriste windows
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

// Ukljuci potporu za osnovne tipove glm-a: vektore i matrice
#include <glm/glm.hpp>

// Ukljuci funkcije za projekcije, transformacije pogleda i slicno
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Object.h"
#include "Renderer.h"
#include "BSplineCurve.h"
#include "Animate.h"

// Ako ste na Ubuntu-u i program se odmah po pokretanju srusi sa segmentation fault,
// radi se o poznatom bugu:
// https://bugs.launchpad.net/ubuntu/+source/nvidia-graphics-drivers-319/+bug/1248642
// potreban je sljedeci define i program treba linkati s -lpthread:
#ifndef _WIN32
#define LINUX_UBUNTU_SEGFAULT
#endif

#ifdef LINUX_UBUNTU_SEGFAULT
//ss1
#include <pthread.h>
#endif


//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint sub_width = 1024, sub_height = 1024;

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myMouseWheel(int button, int dir, int x, int y);

Object object;
Renderer renderer = Renderer();
Camera camera = Camera();
BSplineCurve curve;
Animate animate;

//prati poziciju kursora
float lastx = sub_width / 2.0f;
float lasty = sub_height / 2.0f;
float xoffset, yoffset;
bool firstMouse = true;
bool rightClick = false;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool init_data(char ** argv); // nasa funkcija za inicijalizaciju podataka

//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char ** argv)
{
	// Sljedeci blok sluzi kao bugfix koji je opisan gore
#ifdef LINUX_UBUNTU_SEGFAULT
	//ss2
	int i = pthread_getconcurrency();
#endif

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(sub_width, sub_height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Glut OpenGL Prozor");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutMouseWheelFunc(myMouseWheel);

	glewExperimental = GL_TRUE;
	glewInit();

	if (!init_data(argv)) return 1;

	// Omogući uporabu Z-spremnika
	glEnable(GL_DEPTH_TEST);
	// Prihvaćaj one fragmente koji su bliže kameri u smjeru gledanja
	glDepthFunc(GL_LESS);

	glutMainLoop();
	return 0;
}

//*********************************************************************************
//	Funkcija u kojoj se radi ucitavanja shader programa
//*********************************************************************************

Shader* loadShader(char* path, const char* naziv) {
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;

	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);

	pathVert.append(naziv);
	pathVert.append(".vert");
	pathFrag.append(naziv);
	pathFrag.append(".frag");
	
	return new Shader(pathVert.c_str(), pathFrag.c_str());
}

//*********************************************************************************
//	Funkcija u kojoj se radi inicijalizacija potrebnih VAO-a i VBO-ova. 
//*********************************************************************************

bool init_data(char ** argv)
{
	// Putanja do objekta koji ucitavamo
	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string objPath(dirPath);
	objPath.append("\\frog.obj");

	// Ucitaj objekt s putanje
	object = Object(objPath.c_str());

	// Ucitaj shadere za sjencanje objekta
	object.shader = loadShader(argv[0], "myShader");
	object.setOriginalOrientation(Transform::rotate3D(90, glm::vec3(0.0f, 1.0f, 0.0f)));

	// Postavi kameru u pocetnu tocku

	// Ucitaj krivulju
	std::string curvPath(dirPath);
	curvPath.append("\\curve2.obj");
	curve = BSplineCurve(curvPath.c_str(), 100);
	curve.shader = loadShader(argv[0], "curveShader");

	// Enkapsulacija animacije objekta po zadanoj krivulji
	animate = Animate(&object, &curve);

	// Postavi objekt na pocetnu tocku krivulje i uskladi mu orjentaciju
	animate.updateObject();

	// Postavi renderer
	renderer.addObject(&object);
	renderer.camera = &camera;
	renderer.curve = &curve;

	return true;
}

//*********************************************************************************
//	Osvjezavanje prikaza. (nakon preklapanja prozora) 
//*********************************************************************************

void myDisplay() {

	// vrijeme izmedu dva frame-a
	float currentFrame = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer.camera->ProcessMouseMovement(xoffset, yoffset, rightClick);

	renderer.renderObjects();
	renderer.renderCurve(animate.segment, animate.t);

	animate.nextFrame();
	animate.updateObject();

	glutSwapBuffers();
	glutPostRedisplay();
}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int width, int height)
{
	sub_width = width;                      	// zapamti novu sirinu prozora
	sub_height = height;				// zapamti novu visinu prozora
}

//*********************************************************************************
//	Mis.
//*********************************************************************************

void myMouse(int button, int state, int xPos, int yPos) {

	float x = static_cast<float>(xPos);
	float y = static_cast<float>(yPos);

	if (firstMouse) {
		lastx = x;
		lasty = y;
		firstMouse = false;
	}
	xoffset = x - lastx;
	yoffset = lasty - y;
	lastx = x;
	lasty = y;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN ) {
		rightClick = true;
		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		rightClick = false;
		glutPostRedisplay();
	}
}


//*********************************************************************************
//	Tastatura
//*********************************************************************************

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	float cameraSpeed = 0.1 * camera.MovementSpeed;

	switch (theKey) {
		case 27: //escape key
			exit(0);
		case 'w':
			camera.ProcessKeyboard(FORWARD, deltaTime);
			break;
		case 's':
			camera.ProcessKeyboard(BACKWARD, deltaTime);
			break;
		case 'a':
			camera.ProcessKeyboard(LEFT, deltaTime);
			break;
		case 'd':
			camera.ProcessKeyboard(RIGHT, deltaTime);
			break;
		case 'q':
			camera.ProcessKeyboard(UP, deltaTime);
			break;
		case 'y':
			camera.ProcessKeyboard(DOWN, deltaTime);
			break;
	}
	glutPostRedisplay();
}

void myMouseWheel(int button, int dir, int x, int y) {
	camera.ProcessMouseScroll(static_cast<float>(dir*y));
	glutPostRedisplay();
}

