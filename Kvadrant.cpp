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
	//this->igrac = nullptr;
	this->kraj = false;

}

Kvadrant::~Kvadrant() {
	for (Objekat* objekat : objekti) {
		delete objekat;
	}
	objekti.clear();
	//igrac = nullptr;
}