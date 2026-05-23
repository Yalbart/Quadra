#include "Kvadrant.h"
#include "Menadzer.h"
#include "InputHendler.h"
#include "Renderer.h"
//#include "Platforma.h"
//#include "Merdevine.h"
//#include "Neprijatelj.h"

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
	//if(igrac){igrac->procesInputa(inputi)}
}

void Kvadrant::update(float deltaTime) {
	for (Objekat* objekat : objekti) {
		objekat->update(deltaTime);
	}
	//proveriKolizije();
	//proveriKraj(); il kako god se vec zove ovo
}

void Kvadrant::render() {
	for (Objekat* objekat : objekti)
		objekat->render(*renderer);
}

bool Kvadrant::jelPredjen() const {
	return kraj;
}

void Kvadrant::kontrolaKolizija() {
	//if (!igrac) 
	// return;

}