#pragma once
#include "Renderer.h"
#include "Menadzer.h"
#include "InputHendler.h"
#include "Nivo.h"

class Igra {
public:
	Igra(int argc, char** argv);
	~Igra();
	void run();
private:
	Renderer* renderer;
	Menadzer* menadzerResursa;
	InputHendler* inputHendler;
	Nivo* nivo;
	bool jelRun;
	int sirinaProz;
	int visinaProz;
	float lastTime;
	float deltaTime;

	void procesInputa();
	void update();
	void render();
	void napraviProzor(int argc, char** argv);

	static Igra* instanca;
	static void displejCallback();
	static void idleCallback();
	static void doleCallback(unsigned char dugme, int x , int y);
	static void goreCallback(unsigned char dugme, int x , int y);
	static void specDoleCallback(int dugme, int x , int y);
	static void specGoreCallback(int dugme, int x, int y);
};