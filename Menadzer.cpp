#include "Menadzer.h"
#include <iostream>

Menadzer::Menadzer() {

}

Menadzer::~Menadzer() {
	clear();
	//pozivamo clear funkciju koja brise sve vrednosti iz mape tekstura jednu po jednu
}


//funkcija koja sluzi za ubacivanje teksture u mapu, tj malu "bazu" tekstura, prvo proverava da li je
//tekstura vec ubacena tj load-ovana i da li je u mapi, ukoliko nije ubacuje je, loaduje i vraca
// dereferencirani pokazivac tj samu teksturu
Texture& Menadzer::getTexture(const std::string& filepath) {
	auto i = teksture.find(filepath);
	//auto koristim jer me ne zanima tip, neka kompajler sam odredi nije mi bitno (par vrednosti iz mape je)
	if (i != teksture.end()) {
		return *i->second;
		//veoma prosto, ukoliko data vrednost "filepath" postoji u mapi, dakle nije jednaka "kraju" mape
		//vrati dereferenciranu vrednost (ne kljuc, to bi bila prva vrednost ili first, zato pise second)
	}

	Texture* tekstura = new Texture();
	tekstura->load(filepath);
	teksture[filepath] = tekstura;
	return *tekstura;
	//ovo se izvrsi samo ukoliko tekstura nije vec u mapi, dakle onda pozivamo konstruktor pomocu new
	//uzimamo adresu novokreirane teksture i stavljamo tj pokazujemo na tu adresu sa pokazivacem "tekstura"
	//koristimo load metodu sa novokreirane teksture i ubacujemo fajl u nju
	//na kraju u mapu "teksture" dodajemo novu teksturu i vracamo dereferenciranu vrednost pokazivaca
	//tj samu teksturu
}



void Menadzer::clear() {
	for (auto& par : teksture) {
		delete(par.second);
		//veoma prosti for loop koji prolazi kroz sve parove u mapi i brise vrednosti svakom paru
	}
}