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

//BITNO!!! u trenutnom buildu (a mozda tako i ostane) u kvadrant treba PRVO dodati MERDEVINE
//a tek ONDA platforme, u sustini nastaje jedan verovatno lako resiv bug gde igrac propada kroz platformu
//jer se prvo gleda kolizija sa merdevinama a tek onda platformor svaki frejm
// dakle redosled dodavanja objekata u kvadrant je 1.MERDEVINE pa onda 2.PLATFORME
void Nivo::ucitajKvadrant() {

	float sk = sirinaProz / 2.0f; //sk - sirina kvadranta, sluzi za scale-ing
	float vk = visinaProz / 2.0f; //vk - visina kvadranta, sluzi isto za scale-ing
	float horpomeraj = sk/16.0f; //ovo sluzi za dizajn nivoa, u sustini jedna kockica
	float verpomeraj = vk / 12.0f; //isto samo visina jedne kkockice u kvadrantu...

	//pravljenje objekata kvadranta...
	Kvadrant* k1 = new Kvadrant(renderer, resursi,vec2(0.0f,0.0f)); // gore levo, prvi kvadrant 
	Kvadrant* k2 = new Kvadrant(renderer, resursi, vec2(sk, 0.0f)); // gore desno, drugi kvadrant
	Kvadrant* k3 = new Kvadrant(renderer, resursi, vec2(sk, vk)); //dole desno
	Kvadrant* k4 = new Kvadrant(renderer, resursi, vec2(0.0f, vk)); //dole levo

	//gore levo pocinje igrac
	Igrac* igrac = new Igrac(vec2(0.0f, 0.0f), vec2(sk/32.0f, vk/24.0f));

	//!!!PRVI KVADRANT OBJEKTI!!!:
	//! UPOZORENJE! SPAGHETTI KOD! Level dizajn je HARDKODOVAN! Za sad barem... nemam drugo resenje, a ova funkcija se poziva i pokrece samo jedan put,pa iako je ruzno za citanje, nije toliko lose...
	//takodje za napomenuti, ja kvadrant delim na 16x12 i u odnosu na to dizajniram kvadrant
	//lakse mi je tako, nego odokativno il da sad racunam neke float vrednosti i pozicije...
	//prolazim kroz pozicije kao kroz matricu, zato je kod ovako ruzan, moja ideja a bolju trenutno nemam.


	for (int j = 4; j < 5; j++) {
		for (int i = 0; i < 16; i++) {
			if (i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 12) {
				k1->dodajObjekat(new Neprijatelj(vec2(horpomeraj * i + sk / 32.0f -6.0f, verpomeraj * j), vec2(12.0f, 12.0f), Kretanje::Vertikalno, 50.0f, vk / 6.0f, true));
				k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 24.0f)));
			}}}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			if (i != 3 && i != 6 && i != 10 && i != 11 && i != 13 && i != 14) {
				if (j != 0 && j != 1)
					if (i != 15)
						k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 12.0f)));
					else if (j == 2) {
						Platforma* prviRedPlatf = new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 32.0f));
						prviRedPlatf->vrhMerdevina(true);
						Merdevine* prviRedMerdev = new Merdevine(vec2(horpomeraj * i+5.0f, verpomeraj * j),vec2(sk/20.0f,vk/3.0f));
						k1->dodajObjekat(prviRedMerdev);
						k1->dodajObjekat(prviRedPlatf);
					}}

			else{if (j == 3)
					k1->dodajObjekat(new Neprijatelj(vec2(horpomeraj* i, verpomeraj* j), vec2(sk / 16.0f, vk / 12.0f), Kretanje::Horizontalno, 0.0f, 0.0f));
				if (j == 2) {
					if (i != 11 && i != 14)
						k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 32.0f),Kretanje::Horizontalno,25.0f,horpomeraj));
					 }}}

		for (int j = 6; j < 8; j++) {
			if (i != 0)
				k1->dodajObjekat(new Platforma(vec2(horpomeraj*i,verpomeraj*j),vec2(sk/16.0f,vk/12.0f)));
			else {
				Merdevine* drugiRedMerdev = new Merdevine(vec2(horpomeraj * i + 5.0f, verpomeraj * j), vec2(sk / 20.0f, vk / 6.0f));
				k1->dodajObjekat(drugiRedMerdev);
			}}}





	Platforma* platformaZaMerdevine = new Platforma(vec2(0.0f,40.0f),vec2(sk / 16.0f, vk / 32.0f));
	platformaZaMerdevine->vrhMerdevina(true);
	Merdevine* merdevine1 = new Merdevine(vec2(0.0f, 40.0f), vec2(16.0f, 64.0f));
	Neprijatelj* neprijatelj1 = new Neprijatelj(vec2(sk * 0.5f, vk - 64.0f), vec2(32.0f, 32.0f), Kretanje::Horizontalno, 100.0f, sk *0.2f,true);
	Cilj* cilj1 = new Cilj(vec2(sk - 20.0f, vk - 64.0f), vec2(32.0f, 32.0f));

	



	//dodajem prvo merdevine pa onda platforme
	
	k1->dodajObjekat(igrac);
	k1->setujIgraca(igrac);
	
	k1->dodajObjekat(neprijatelj1);

	k1->dodajObjekat(cilj1);
	k1->setujCilj(cilj1);




	//KONACNO DODAVANJE KVADRANTA NA NIVO
	kvadranti.push_back(k1);
	kvadranti.push_back(k2);
	kvadranti.push_back(k3);
	kvadranti.push_back(k4);
}