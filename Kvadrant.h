#pragma once
#include "Objekat.h"
#include"Igrac.h"
#include "Cilj.h"
#include "Menadzer.h"
#include "InputHendler.h"
#include "Renderer.h"
#include "Platforma.h"
#include "Merdevine.h"
#include "Neprijatelj.h"

#include <iostream>
#include <vector>

class Renderer;
class Menadzer;
class InputHendler;

class Kvadrant {
public:
	Kvadrant(Renderer* renderer, Menadzer* menadzerResursa, vec2 pomeraj);
	~Kvadrant();
	void procesInputa(InputHendler* inputi);
	void update(float deltaTime);
	void render();
	bool jelPredjen() const;
	void dodajObjekat(Objekat* objekat);
	void setujIgraca(Igrac* igrac);
	void setujCilj(Cilj* cilj);
	vec2 pomeraj; // dodato 20.06. ovo sluzi za scale-ovanje sa dimenzijama ekrana i crtanje kvadranta na ekranu, pomeraj izmedju kvadranta
private:
	vector<Objekat*> objekti;
	Igrac* igrac;
	Renderer* renderer;
	Menadzer* resursi;
	Cilj* cilj;
	bool predjen; // flag da li je kvadrant predjen, trebace za tajmer
	void kontrolaKolizija();
	void KontrolaIzlazaIzKvadranta();
};