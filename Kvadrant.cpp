#include "Kvadrant.h"
#include "Menadzer.h"
#include "InputHendler.h"
#include "Renderer.h"
#include "Platforma.h"
#include "Merdevine.h"
#include "Neprijatelj.h"
#include <iostream>

Kvadrant::Kvadrant(Renderer* renderer, Menadzer* resursi) {
	this->renderer = renderer;
	this->resursi = resursi;
	this->igrac = nullptr;
	this->kraj = false;

}

Kvadrant::~Kvadrant() {
	for (Objekat* objekat : objekti) {
		delete objekat;
	}
	objekti.clear();
	igrac = nullptr;
}

void Kvadrant::procesInputa(InputHendler* inputi) {
	if (igrac)
		igrac->procesInputa(inputi);
}

void Kvadrant::update(float deltaTime) {
	for (Objekat* objekat : objekti) {
		objekat->update(deltaTime);
	}
	/*if (igrac) {
		std::cout << "Igrac: " << igrac->poz.y << " velicina: " << igrac->vel.y
			<< " Zemlja: " << 368.0f << std::endl;
	}
	*/
	kontrolaKolizija();
	KontrolaIzlazaIzKvadranta(); //il kako god se vec zove ovo
}

void Kvadrant::render() {
	for (Objekat* objekat : objekti)
		objekat->render(*renderer);
}

bool Kvadrant::jelPredjen() const {
	return kraj;
}

void Kvadrant::kontrolaKolizija() {
	if (!igrac) 
		return;
	for (Objekat* objekat : objekti) {
		if (objekat == igrac) continue;
		//AABB kolizija
		bool hit = igrac->poz.x < objekat->poz.x + objekat->vel.x &&
			igrac->poz.x + igrac->vel.x > objekat->poz.x &&
			igrac->poz.y < objekat->poz.y + objekat->vel.y &&
			igrac->poz.y + igrac->vel.y > objekat->poz.y;
	
		if (hit) {
			Merdevine* merdevine = dynamic_cast<Merdevine*>(objekat);
			if (merdevine) {
				merdevine->kolizija(igrac);
				continue; // skip platform resolution for ladders
			}
			objekat->kolizija(igrac);
		}
	}
}

void Kvadrant::KontrolaIzlazaIzKvadranta() {
	//dodacu kasnije
}

void Kvadrant::dodajObjekat(Objekat* objekat) {
	objekti.push_back(objekat);
}

void Kvadrant::setujIgraca(Igrac* igrac) {
	this->igrac = igrac;
}