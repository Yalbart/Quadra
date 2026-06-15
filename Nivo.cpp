#include "Nivo.h"
#include "Renderer.h"
#include "Menadzer.h"
#include "Kvadrant.h"
#include "InputHendler.h"
#include "Igrac.h"
#include "Merdevine.h"
#include "Platforma.h"
#include "Neprijatelj.h"
#include "Kretanje.h"

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

	Kvadrant* k1 = new Kvadrant(renderer, resursi);

	//gore desno pocinje igrac
	Igrac* igrac = new Igrac(vec2(50.0f, 50.0f), vec2(32.0f, 32.0f));
	k1->dodajObjekat(igrac);
	k1->setujIgraca(igrac);


//BITNO!!! u trenutnom buildu (a mozda tako i ostane) u kvadrant treba PRVO dodati MERDEVINE
//a tek ONDA platforme, u sustini nastaje jedan verovatno lako resiv bug gde igrac propada kroz platformu
//jer se prvo gleda kolizija sa merdevinama a tek onda platformor svaki frejm
// dakle redosled dodavanja objekata u kvadrant je 1.MERDEVINE pa onda 2.PLATFORME
	Platforma* platformaZaMerdevine = new Platforma(vec2(100.0f,272.0f),vec2(120.0f,16.0f));
	platformaZaMerdevine->vrhMerdevina(true);

	//dodajem prvo merdevine pa onda platforme
	k1->dodajObjekat(new Merdevine(vec2(100.0f, 272.0f), vec2(16.0f, 96.0f)));
	k1->dodajObjekat(new Platforma(vec2(0.0f, 368.0f), vec2(400.0f, 32.0f)));
	k1->dodajObjekat(platformaZaMerdevine);
	k1->dodajObjekat(new Platforma(vec2(250.0f, 176.0f), vec2(120.0f, 16.0f)));
	
	k1->dodajObjekat(new Neprijatelj(vec2(200.0f, 336.0f), vec2(32.0f, 32.0f), Kretanje::Horizontalno, 100.0f, 150.0f));

	kvadranti.push_back(k1);
	kvadranti.push_back(new Kvadrant(renderer, resursi));
	kvadranti.push_back(new Kvadrant(renderer, resursi));
	kvadranti.push_back(new Kvadrant(renderer, resursi));
}