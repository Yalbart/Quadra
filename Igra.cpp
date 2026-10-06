#include "Igra.h"
#include<iostream>

Igra* Igra::instanca = nullptr;

Igra::Igra(int argc, char** argv) : stanje(StanjeIgre::GlavniMeni){
	jelRun = true;
	lastTime = 0.0f;
	deltaTime = 0.0f;
	instanca = this;
	napraviProzor(argc, argv);

	renderer = new Renderer(sirinaProz, visinaProz);
	menadzerResursa = new Menadzer();
	inputHendler = new InputHendler();
	nivo = new Nivo(renderer,menadzerResursa, sirinaProz, visinaProz, &brSkupljenogSira);

	meniLogo = &menadzerResursa->getTexture("teksture/meni_logo.png");
	meniKlikni = &menadzerResursa->getTexture("teksture/meni_klikni.png");
	texPobeda = &menadzerResursa->getTexture("teksture/pobeda.png");
	texPoraz = &menadzerResursa->getTexture("teksture/poraz.png");


	//08.09.2026.
	hudZivoti = &menadzerResursa->getTexture("teksture/srce.png");
	hudSir = &menadzerResursa->getTexture("teksture/sir.png");
	for (int i = 0; i <= 9; i++) {
		brojevi[i] = &menadzerResursa->getTexture("teksture/br" + std::to_string(i) + ".png");
	}

	stanje = StanjeIgre::IgraSe;
	zivoti = 3;
	
	

	//13.07.
	
	
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

	int sirPomoc1 = 0;
	if (brSkupljenogSira > brSiraPredhodniFrejm) {
		zivoti++;
		brSiraPredhodniFrejm++;
	}

	if (brSkupljenogSira >= 4) {
		stanje = StanjeIgre::Pobeda;
		cout << "POBEDA!!!" << endl;
	}

	//int pomocniSirCounter = brSkupljenogSira;
	//brSkupljenogSira = nivo->brojSakupljenihSireva();
	//if (brSkupljenogSira >= 4)
	//	stanje = StanjeIgre::Pobeda;
	//if (brSkupljenogSira > pomocniSirCounter) {
	//	zivoti++;
	//}

	//cout <<"ZIVOTI:" << this->zivoti << endl;
	//cout << "Pojedenih sireva br: " << this->brSkupljenogSira << endl;
}

void Igra::render() {
	renderer->clear();
	if (stanje == StanjeIgre::IgraSe) {
		nivo->render();
		//renderer->crtajLinije(sirinaProz, visinaProz);
		renderHUD();
	}
	else if (stanje == StanjeIgre::IzgubioSi) {

	}
	else if (stanje == StanjeIgre::Pobeda) {

	}
	
	glutSwapBuffers();
}

void Igra::renderHUD() {
	float vel = 32.0f;
	float pad = 10.0f;

	//srce
	renderer->drawSprite(*hudZivoti, glm::vec2(pad, pad), glm::vec2(vel * 2, vel));

	//br zivota
	renderer->drawSprite(*brojevi[zivoti], glm::vec2(pad + vel * 2 + pad, pad), glm::vec2(vel, vel));

	//sir
	renderer->drawSprite(*hudSir, glm::vec2(pad + vel * 4, pad), glm::vec2(vel * 2, vel));

	//br sira
	renderer->drawSprite(*brojevi[brSkupljenogSira], glm::vec2(pad + vel * 6 + pad, pad), glm::vec2(vel, vel));
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
	if (zivoti <= 0) {
		stanje = StanjeIgre::IzgubioSi;
		cout << "GEJM OVER!!!" << endl;
	}
	else {
		nivo->resetSveIgrace();
		cout << "UDAREN SI! Broj zivota preostalo: " << zivoti << endl;
	}
}
void Igra::pojedenSir() {
	brSkupljenogSira++;
	std::cout << "Skupljen sir!!! br sireva: " << brSkupljenogSira << std::endl;
	if (zivoti < 3) zivoti++;
	if (brSkupljenogSira >= 4)
		stanje = StanjeIgre::Pobeda;
}

void Igra::renderGlavniMeni() {
	float logoSirina = sirinaProz * 0.5f;
	float logoVisina = visinaProz * 0.4f;
	renderer->drawSprite(*meniLogo, vec2(sirinaProz / 2.0f - logoSirina / 2.0f, visinaProz * 0.2f), vec2(logoSirina, logoVisina));

	float klikSirina = sirinaProz * 0.4f;
	float klikVisina = visinaProz * 0.1f;
	renderer->drawSprite(*meniKlikni, vec2(sirinaProz / 2.0f / klikSirina / 2.0f, visinaProz * 0.75f),vec2(klikSirina,klikVisina));
}