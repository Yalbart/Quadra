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

Nivo::Nivo(Renderer* renderer, Menadzer* resursi,int sirinaProz,int visinaProz ) {
	this->renderer = renderer;
	this->resursi = resursi;
	this->trenutniKvadrant = 0;
	this->sirinaProz = sirinaProz;
	this->visinaProz = visinaProz;
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
	//if (trenutniKvadrant < (int)kvadranti.size())
		//kvadranti[trenutniKvadrant]->render();
	for (Kvadrant* k : kvadranti) {
		k->render(k->pomeraj);
	}

}

bool Nivo::jelPredjen() const {
	return trenutniKvadrant >= (int)kvadranti.size();
	
}

void Nivo::ucitajKvadrant() {

	float sk = sirinaProz / 2.0f; //sk - sirina kvadranta, sluzi za scale-ing
	float vk = visinaProz / 2.0f; //vk - visina kvadranta, sluzi isto za scale-ing

	Kvadrant* k1 = new Kvadrant(renderer, resursi,vec2(0.0f,0.0f)); // gore levo, prvi kvadrant 
	Kvadrant* k2 = new Kvadrant(renderer, resursi, vec2(sk, 0.0f)); // gore desno, drugi kvadrant
	Kvadrant* k3 = new Kvadrant(renderer, resursi, vec2(sk, vk)); //dole desno
	Kvadrant* k4 = new Kvadrant(renderer, resursi, vec2(0.0f, vk)); //dole levo

	//gore levo pocinje igrac
	Igrac* igrac = new Igrac(vec2(sk* 0.1f, vk * 0.1f), vec2(32.0f, 32.0f));
	k1->dodajObjekat(igrac);
	k1->setujIgraca(igrac);


//BITNO!!! u trenutnom buildu (a mozda tako i ostane) u kvadrant treba PRVO dodati MERDEVINE
//a tek ONDA platforme, u sustini nastaje jedan verovatno lako resiv bug gde igrac propada kroz platformu
//jer se prvo gleda kolizija sa merdevinama a tek onda platformor svaki frejm
// dakle redosled dodavanja objekata u kvadrant je 1.MERDEVINE pa onda 2.PLATFORME
	Platforma* platformaZaMerdevine = new Platforma(vec2(100.0f,272.0f),vec2(120.0f,16.0f));
	platformaZaMerdevine->vrhMerdevina(true);
	Merdevine* merdevine1 = new Merdevine(vec2(sk* 0.25f, vk *0.5f), vec2(16.0f, vk* 0.25f));
	Platforma* platforma1 = new Platforma(vec2(0.0f, vk - 32.0f), vec2(sk, 32.0f));
	Platforma* platforma2 = new Platforma(vec2(sk* 0.6f, vk * 0.5f), vec2(sk * 0.3f, 16.0f));
	Neprijatelj* neprijatelj1 = new Neprijatelj(vec2(sk * 0.5f, vk - 64.0f), vec2(32.0f, 32.0f), Kretanje::Horizontalno, 100.0f, sk *0.2f);
	Cilj* cilj1 = new Cilj(vec2(sk - 20.0f, vk - 64.0f), vec2(32.0f, 32.0f));



	//dodajem prvo merdevine pa onda platforme
	k1->dodajObjekat(merdevine1);
	k1->dodajObjekat(platforma1);
	k1->dodajObjekat(platformaZaMerdevine);
	k1->dodajObjekat(platforma2);
	
	k1->dodajObjekat(neprijatelj1);

	k1->dodajObjekat(cilj1);
	k1->setujCilj(cilj1);

	kvadranti.push_back(k1);
	kvadranti.push_back(k2);
	kvadranti.push_back(k3);
	kvadranti.push_back(k4);
}