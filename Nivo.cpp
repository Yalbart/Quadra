#include "Nivo.h"
#include "Renderer.h"
#include "Menadzer.h"
#include "Kvadrant.h"
#include "InputHendler.h"

Nivo::Nivo(Renderer* renderer, Menadzer* resursi ) {
	this->renderer = renderer;
	this->resursi = resursi;
	this->trenutniKvadrant = 0;
	ucitajKvadrant();
}

Nivo::~Nivo() {
	for (Kvadrant* kvadrant : kvadranti) {
		delete kvadrant;
	}
	kvadranti.clear();
}

void Nivo::procesInputa(InputHendler* inputi) {
	if (trenutniKvadrant < (int)kvadranti.size())
		kvadranti[trenutniKvadrant]->procesInputa(inputi);
}

void Nivo::update(float deltaTime){
	if (trenutniKvadrant >= (int)kvadranti.size()) return;
	kvadranti[trenutniKvadrant]->update(deltaTime);
	if (kvadranti[trenutniKvadrant]->jelPredjen())
		trenutniKvadrant++;
}

void Nivo::render() {
	if (trenutniKvadrant < (int)kvadranti.size())
		kvadranti[trenutniKvadrant]->render();
}

bool Nivo::jelPredjen() const {
	return trenutniKvadrant >= (int)kvadranti.size();
	
}

void Nivo::ucitajKvadrant() {
	kvadranti.push_back(new Kvadrant(renderer, resursi));
	kvadranti.push_back(new Kvadrant(renderer, resursi));
	kvadranti.push_back(new Kvadrant(renderer, resursi));
	kvadranti.push_back(new Kvadrant(renderer, resursi));
}