#include "Igra.h"
#include<iostream>

Igra* Igra::instanca = nullptr;

Igra::Igra(int argc, char** argv) {
	jelRun = true;
	sirinaProz = 800;
	visinaProz = 450;
	lastTime = 0.0f;
	deltaTime = 0.0f;
	instanca = this;
	napraviProzor(argc, argv);

	renderer = new Renderer(sirinaProz, visinaProz);
	menadzerResursa = new Menadzer();
	inputHendler = new InputHendler();
	nivo = new Nivo(renderer,menadzerResursa);
}

Igra::~Igra() {
	delete renderer;
	delete menadzerResursa;
	delete inputHendler;
	delete nivo;
}

void Igra::run() {
	glutMainLoop();
}

void Igra::procesInputa() {
	nivo->procesInputa(inputHendler);
}

void Igra::update() {
	float trenVreme = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = trenVreme - lastTime;
	lastTime = trenVreme;

	if (deltaTime > 0.05f) deltaTime = 0.05f; //ovo ce da cappuje delta vreme na 0.05, resava jedan bug kad se pokrene program.

	nivo->update(deltaTime);
}

void Igra::render() {
	renderer->clear();
	nivo->render();
	glutSwapBuffers();
}

void Igra::napraviProzor(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(sirinaProz, visinaProz);
	glutCreateWindow("TMQuadro");
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	GLenum glew = glewInit();
	if (glew != GLEW_OK) {
		std::cout << "GLEW ne radi :( : " << glewGetErrorString(glew) << std::endl;
	}

	glutDisplayFunc(displejCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(doleCallback);
	glutKeyboardUpFunc(goreCallback);
	glutSpecialFunc(specDoleCallback);
	glutSpecialUpFunc(specGoreCallback);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);//ovo je vise preventivno, da se ne bi zaglavljivali/pamtili inputi
}

void Igra::displejCallback() {
	instanca->render();
}

void Igra::idleCallback() {
	instanca->update();
	instanca->procesInputa();
	glutPostRedisplay();
}

void Igra::doleCallback(unsigned char dugme, int x, int y) {
	instanca->inputHendler->dole(dugme);
}

void Igra::goreCallback(unsigned char dugme, int x, int y) {
	instanca->inputHendler->gore(dugme);
}

void Igra::specDoleCallback(int dugme, int x, int y) {
	std::cout << "KEY DOWN fired: " << dugme << std::endl;
	instanca->inputHendler->specDole(dugme);
}

void Igra::specGoreCallback(int dugme, int x, int y) {
	std::cout << "KEY UP fired: " << dugme << std::endl;
	instanca->inputHendler->specGore(dugme);
}