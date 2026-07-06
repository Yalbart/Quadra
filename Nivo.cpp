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

Nivo::Nivo(Renderer* renderer, Menadzer* resursi,int sirinaProz,int visinaProz ): igrac(nullptr) {
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
	
	for (Kvadrant* k : kvadranti) {
		k->procesInputa(inputi);
	}
	
	//if (trenutniKvadrant < (int)kvadranti.size())
		//kvadranti[trenutniKvadrant]->procesInputa(inputi);
}

void Nivo::update(float deltaTime){
	
	for (Kvadrant* k : kvadranti) {
		k->update(deltaTime);
	}
	
	//if (trenutniKvadrant >= (int)kvadranti.size()) return;
	//kvadranti[trenutniKvadrant]->update(deltaTime);
	//if (kvadranti[trenutniKvadrant]->jelPredjen())
	//	trenutniKvadrant++;
}

void Nivo::render() {
	//if (trenutniKvadrant < (int)kvadranti.size())
		//kvadranti[trenutniKvadrant]->render();
	for (Kvadrant* k : kvadranti) {
		k->render();
	}
	igrac->render(*renderer);

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
	float verpomeraj = vk / 12.0f; //isto samo visina jedne kockice u kvadrantu...
	vec2 k2pomeraj(sk, 0.0f);


	//pravljenje objekata kvadranta...
	Kvadrant* k1 = new Kvadrant(renderer, resursi,vec2(0.0f,0.0f)); // gore levo, prvi kvadrant 
	Kvadrant* k2 = new Kvadrant(renderer, resursi, vec2(sk, 0.0f)); // gore desno, drugi kvadrant
	Kvadrant* k3 = new Kvadrant(renderer, resursi, vec2(sk, vk)); //dole desno
	Kvadrant* k4 = new Kvadrant(renderer, resursi, vec2(0.0f, vk)); //dole levo

	//gore levo pocinje igrac
	igrac = new Igrac(vec2(horpomeraj+sk/32.0f, verpomeraj+vk/24.0f), vec2(sk/32.0f, vk/24.0f));
	k1->setujIgraca(igrac);
	k2->setujIgraca(igrac);
	k3->setujIgraca(igrac);
	k4->setujIgraca(igrac);

	//! UPOZORENJE! SPAGHETTI KOD! Level dizajn je HARDKODOVAN! Za sad barem... nemam drugo resenje, a ova funkcija se poziva i pokrece samo jedan put,pa iako je ruzno za citanje, nije toliko lose...
	//takodje za napomenuti, ja kvadrant delim na 16x12 i u odnosu na to dizajniram kvadrant
	//lakse mi je tako, nego odokativno il da sad racunam neke float vrednosti i pozicije...
	//prolazim kroz pozicije kao kroz matricu, zato je kod ovako ruzan, moja ideja a bolju trenutno nemam.

	//jos jedna brza napomena NAMING CONVENTION: varijable ce nositi imena PRVIRED DRUGIRED TRECIRED ITD...
	//iako cu pisati da redova ima 12 (od nultog 0 do jedanaestog 11) u imenima varijabli
	//poput prviRedPlatf ili treciRedMerdev1 ce redovi znaciti igrivi delovi kvadranta umesto
	//bukvalnog reda u mojoj 16x12 matrici tj gridu na osnovu kojeg dizajniram nivo

	//!!!PRVI KVADRANT OBJEKTI!!!:

	//kvadrant podeljen na 16x12 matricu, prvo pravim platforme
	for (int i = 0; i < 16; i++) {
		for (int j = 2; j < 4; j++) {
			if (i != 3 && i != 6 && i != 10 && i != 11 && i != 13 && i != 14 && i != 15) 
				k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 12.0f)));
			
			//bodlje i pomerajuce platforme u 2. i 3. redu
			else if (i != 15) {
				if (j == 3)
					k1->dodajObjekat(new Neprijatelj(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 12.0f), Kretanje::Horizontalno, 0.0f, 0.0f));
				if (j == 2) {
					if (i != 11 && i != 14)
						k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 32.0f), Kretanje::Horizontalno, 25.0f, horpomeraj));
				}
			}
		}

		//red broj 4, prvi red cevki sa kapljicama
		if (i == 2 || i == 4 || i == 5 || i == 7 || i == 9 || i == 12) {
			k1->dodajObjekat(new Neprijatelj(vec2(horpomeraj * i + sk / 32.0f - 6.0f, verpomeraj * 4), vec2(12.0f, 12.0f), Kretanje::Vertikalno, 50.0f, vk / 6.0f, true));
			k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * 4), vec2(sk / 16.0f, vk / 24.0f)));
		}

		//redovi 6 i 7 platforme kockice, takodje merdevine za nizi nivo
		for (int j = 6; j < 8; j++) {
			if (i != 0)
				k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 12.0f)));
			else {
				Merdevine* drugiRedMerdev = new Merdevine(vec2(horpomeraj * i + 5.0f, verpomeraj * j), vec2(sk / 20.0f, vk / 6.0f));
				k1->dodajObjekat(drugiRedMerdev);
			}
		}

		//red broj 8 cevke i kapljice, brzinaKaplj sluzi samo da tri uzastopne cevke na mestima 10 11 i 12 imaju razlicite brzine
		if (i == 2 || i > 7 && i != 9 && i < 13) {
			int brzinaKaplj = 70.0f;
			if (i == 10)
				brzinaKaplj += 10.0f;
			else if (i == 11)
				brzinaKaplj += 12.5f;
			else
				brzinaKaplj += 15.0f;
			k1->dodajObjekat(new Neprijatelj(vec2(horpomeraj * i + sk / 32.0f - 6.0f, verpomeraj * 8), vec2(12.0f, 12.0f), Kretanje::Vertikalno, brzinaKaplj, vk / 4.0f, true));
			k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * 8), vec2(sk / 16.0f, vk / 24.0f)));
		}

		//redovi 9 i 10 platforme kockice
		for (int j = 9; j < 11; j++) {
			if (i < 2 || i>4 && i < 8 || i>13)
				k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 12.0f)));
		}


		//red 11 tj. poslednji red prvog kvadranta, sve platforme kockice
		k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * 11), vec2(sk / 16.0f, vk / 12.0f)));

	}

		//merdevine 2. i 3. red
		Platforma* prviRedPlatf = new Platforma(vec2(horpomeraj * 15, verpomeraj * 2), vec2(sk / 16.0f, vk / 32.0f));
		prviRedPlatf->vrhMerdevina(true);
		Merdevine* prviRedMerdev = new Merdevine(vec2(horpomeraj * 15 + 5.0f, verpomeraj * 2), vec2(sk / 20.0f, vk / 3.0f));
		k1->dodajObjekat(prviRedMerdev);
		k1->dodajObjekat(prviRedPlatf);

		//merdevine 9. red broj 1
		Platforma* treciRedPlatf1 = new Platforma(vec2(horpomeraj * 4, verpomeraj * 9), vec2(sk / 16.0f, vk / 32.0f));
		treciRedPlatf1->vrhMerdevina(true);
		Merdevine* treciRedMerdev1 = new Merdevine(vec2(horpomeraj * 4 + 5.0f, verpomeraj * 9), vec2(sk / 20.0f, vk / 6.0f));
		k1->dodajObjekat(treciRedMerdev1);
		k1->dodajObjekat(treciRedPlatf1);

		//merdevine 9. red broj 2
		Platforma* treciRedPlatf2 = new Platforma(vec2(horpomeraj * 13, verpomeraj * 9), vec2(sk / 16.0f, vk / 32.0f));
		treciRedPlatf2->vrhMerdevina(true);
		Merdevine* treciRedMerdev2 = new Merdevine(vec2(horpomeraj * 13 + 5.0f, verpomeraj * 9), vec2(sk / 20.0f, vk / 6.0f));
		k1->dodajObjekat(treciRedMerdev2);
		k1->dodajObjekat(treciRedPlatf2);

		//sir tj. cilj
		Cilj* cilj1 = new Cilj(vec2(horpomeraj*15+22.0f ,verpomeraj*8+16.0f), vec2(16.0f, 16.0f));
		//k1->dodajObjekat(cilj1);
		//k1->setujCilj(cilj1);

	
	//dodajem igraca na kraju, kako bi se renderovao ispred svega ostalog
	k1->dodajObjekat(igrac);
	
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 12; j++) {
			if (i == 0 && j != 8 || j==0||i==15||j==11&&i!=1)
				k2->dodajObjekat(new Platforma(vec2(k2pomeraj.x + horpomeraj * i, k2pomeraj.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
		}

	}


	//KONACNO DODAVANJE KVADRANTA NA NIVO
	kvadranti.push_back(k1);
	kvadranti.push_back(k2);
	kvadranti.push_back(k3);
	kvadranti.push_back(k4);
}