#include "Igra.h"
#include<iostream>

Igra* Igra::instanca = nullptr;

Igra::Igra(int argc, char** argv) {
	jelRun = true;
	lastTime = 0.0f;
	deltaTime = 0.0f;
	instanca = this;
	napraviProzor(argc, argv);

	renderer = new Renderer(sirinaProz, visinaProz);
	menadzerResursa = new Menadzer();
	inputHendler = new InputHendler();
	nivo = new Nivo(renderer,menadzerResursa, sirinaProz, visinaProz);
	stanje = StanjeIgre::IgraSe;
	zivoti = 3;
	brSkupljenogSira = 0;

	//13.07.
	nivo->skupljenSir = [this]() {pojedenSir();};
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
	if (stanje != StanjeIgre::IgraSe)
		return;

	nivo->update(deltaTime);

	//proverava da li je ovaj frejm igrac "ripno" tj bio u koliziji sa neprijateljem
	nivo->jelRipno([this]() {ripIgrac(); });
	if (brSkupljenogSira >= 4)
		stanje = StanjeIgre::Pobeda;
}

void Igra::render() {
	renderer->clear();
	if (stanje == StanjeIgre::IgraSe) {
		nivo->render();
		renderer->crtajLinije(sirinaProz, visinaProz);
	}
	else if (stanje == StanjeIgre::IzgubioSi) {

	}
	else if (stanje == StanjeIgre::Pobeda) {

	}
	
	glutSwapBuffers();
}
	
	//pravi prozor inicira glut i glew
void Igra::napraviProzor(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	sirinaProz = glutGet(GLUT_SCREEN_WIDTH);
	visinaProz = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowSize(sirinaProz, visinaProz);
	glutCreateWindow("TMQuadro");
	//glutFullScreen();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	GLenum glew = glewInit();
	if (glew != GLEW_OK) {
		std::cout << "GLEW ne radi :( : " << glewGetErrorString(glew) << std::endl;
	}
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);//ovo je vise preventivno, da se ne bi zaglavljivali/pamtili inputi
	glutDisplayFunc(displejCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(doleCallback);
	glutKeyboardUpFunc(goreCallback);
	glutSpecialFunc(specDoleCallback);
	glutSpecialUpFunc(specGoreCallback);
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
	if (dugme == 27) //27 je ESC
		exit(0);

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

void Igra::ripIgrac() {
	zivoti--;
	if (zivoti <= 0)
		stanje = StanjeIgre::IzgubioSi;
	else
		nivo->resetSveIgrace();
}

void Igra::pojedenSir() {
	brSkupljenogSira++;
	if (zivoti < 3) zivoti++;
	if (brSkupljenogSira >= 4)
		stanje = StanjeIgre::Pobeda;
}