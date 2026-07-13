#include "Kvadrant.h"


Kvadrant::Kvadrant(Renderer* renderer, Menadzer* resursi, vec2 pomeraj) {
	this->renderer = renderer;
	this->resursi = resursi;
	this->igrac = nullptr;
	this->cilj = nullptr;
	this->predjen = false;
	this->pomeraj = pomeraj;
	

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
		/*if (objekat->jelSkupljenSir) {
			sakupljeniSirevi++;
			objekat->jelSkupljenSir = false;// dodato 13.07. valjda ce mi popraviti dva bug-a koja imam... vezani su za brisanje i nepravilno inkrementiranje sira
		}*/
	}

	kontrolaKolizija();
	//KontrolaIzlazaIzKvadranta(); //il kako god se vec zove ovo

	//dodato 13.07. loop za checkovanje sta treba da se obrise (za ciljeve/sir konkretno)
	objekti.erase(
		std::remove_if(objekti.begin(), objekti.end(), [](Objekat* obj) {
			if (obj->oznacenZaBrisanje) {
				delete obj;
				return true;
			}return false;}),objekti.end());
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
				continue; //ovo je bio pokusaj da popravim bug sa platformama i merdevinama al sam "resio" na drugi nacin a ovo je ostalo ovako jer ne kvari nista
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

Igrac* Kvadrant::getIgraca() {
	return igrac;
}

void Kvadrant::setujCilj(Cilj* cilj) {
	this->cilj = cilj;
}

vector<Objekat*>& Kvadrant::getObjekte() {
	return objekti;
}


