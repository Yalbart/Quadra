#include "Kvadrant.h"


Kvadrant::Kvadrant(Renderer* renderer, Menadzer* resursi) {
	this->renderer = renderer;
	this->resursi = resursi;
	this->igrac = nullptr;
	this->cilj = nullptr;
	this->predjen = false;

}

Kvadrant::~Kvadrant() {
	for (Objekat* objekat : objekti) {
		delete objekat;
	}
	objekti.clear();
	igrac = nullptr;
	cilj = nullptr;
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
	return cilj && cilj->stigaoDoKraja();
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

void Kvadrant::setujCilj(Cilj* cilj) {
	this->cilj = cilj;
}