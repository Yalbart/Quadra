#pragma once
#include "Renderer.h"
#include "Menadzer.h"
#include "InputHendler.h"
#include "Nivo.h"

enum class StanjeIgre {
	GlavniMeni,
	IgraSe,
	IzgubioSi,
	Pobeda
};

class Igra {
public:
	Igra(int argc, char** argv);
	~Igra();
	void run();

	void ripIgrac();
	void pojedenSir();

private:
	Renderer* renderer;
	Menadzer* menadzerResursa;
	InputHendler* inputHendler;
	Nivo* nivo;
	StanjeIgre stanje;
	bool jelRun;
	int zivoti;
	int brSkupljenogSira = 0;
	int brSiraPredhodniFrejm = 0;

	int sirinaProz;
	int visinaProz;
	float lastTime;
	float deltaTime;

	Texture* hudZivoti;
	Texture* hudSir;
	Texture* brojevi[10];
	Texture* meniLogo;
	Texture* meniKlikni;
	Texture* texPobeda;
	Texture* texPoraz;


	void renderHUD();
	void renderGlavniMeni();
	void renderPobeda();
	void renderPoraz();

	void pokreniIgru();
	void vratiNaMeni();

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