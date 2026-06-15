#pragma once
#include "Objekat.h"
#include"Igrac.h"
#include <vector>

class Renderer;
class Menadzer;
class InputHendler;

class Kvadrant {
public:
	Kvadrant(Renderer* renderer, Menadzer* menadzerResursa);
	~Kvadrant();
	void procesInputa(InputHendler* inputi);
	void update(float deltaTime);
	void render();
	bool jelPredjen() const;
	void dodajObjekat(Objekat* objekat);
	void setujIgraca(Igrac* igrac);
private:
	std::vector<Objekat*> objekti;
	Igrac* igrac;
	Renderer* renderer;
	Menadzer* resursi;
	bool kraj;
	void kontrolaKolizija();
	void KontrolaIzlazaIzKvadranta();
};