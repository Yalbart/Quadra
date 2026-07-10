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
#include "Portal.h"

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
	vec2 k2p(sk, 0.0f); //pomeraj za drugi kvadrant, koristi se za podelu ekrana na kvadrante
	vec2 k3p(sk, vk); //pomeraj za treci kvadrant, takodje ovi pomeraji sluze kasnije za rednerovanje po koordinatama lokalnih objekata u kvadrantu i njihove kolizije
	vec2 k4p(0.0f, vk); //pomeraj za cetvrti kvadrant


	//pravljenje objekata kvadranta...
	Kvadrant* k1 = new Kvadrant(renderer, resursi,vec2(0.0f,0.0f)); // gore levo, prvi kvadrant 
	Kvadrant* k2 = new Kvadrant(renderer, resursi, k2p); // gore desno, drugi kvadrant
	Kvadrant* k3 = new Kvadrant(renderer, resursi, k3p); //dole desno
	Kvadrant* k4 = new Kvadrant(renderer, resursi, vec2(0.0f, vk)); //dole levo

	//gore levo pocinje igrac
	igrac = new Igrac(vec2(k3p.x+horpomeraj*3.0f, k3p.y+verpomeraj*10.0f), vec2(sk/32.0f, vk/24.0f));
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
						k1->dodajObjekat(new Platforma(vec2(horpomeraj * i, verpomeraj * j), vec2(sk / 16.0f, vk / 32.0f), Kretanje::Horizontalno, 25.0f, horpomeraj,1));
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
			float brzinaKaplj = 70.0f;
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
		Cilj* sir1 = new Cilj(vec2(horpomeraj*15+22.0f ,verpomeraj*8+16.0f), vec2(16.0f, 16.0f));
		k1->dodajObjekat(sir1);
		k1->setujCilj(sir1);

	
	//dodajem igraca na kraju, kako bi se renderovao ispred svega ostalog
	k1->dodajObjekat(igrac);
	  
	//!!!DRUGI KVADRANT OBJEKTI!!!:
	//! 
		//neprijatelji drugog kvadranta... prvo mravi, pa crvici, pa na kraju veliki brzi mrav
		//njih prvo pravim i dodajem kako bi se prvi renderovali
		Neprijatelj* mrav1 = new Neprijatelj(vec2(k2p.x + horpomeraj * 15 , k2p.y + verpomeraj * 3 + verpomeraj / 2), vec2(horpomeraj / 2, verpomeraj / 2), Kretanje::Horizontalno, 100.0f, horpomeraj * 13, true, -1);
		k2->dodajObjekat(mrav1);
		Neprijatelj* mrav2 = new Neprijatelj(vec2(k2p.x + horpomeraj * 15, k2p.y + verpomeraj * 3 + verpomeraj / 2), vec2(horpomeraj / 2, verpomeraj / 2), Kretanje::Horizontalno, 100.0f, horpomeraj * 13, true, -1, 3.0f * horpomeraj);
		k2->dodajObjekat(mrav2);
		Neprijatelj* mrav3 = new Neprijatelj(vec2(k2p.x + horpomeraj * 15, k2p.y + verpomeraj * 3 + verpomeraj / 2), vec2(horpomeraj / 2, verpomeraj / 2), Kretanje::Horizontalno, 100.0f, horpomeraj * 13, true, -1, 6.0f * horpomeraj);
		k2->dodajObjekat(mrav3);
		Neprijatelj* mrav4 = new Neprijatelj(vec2(k2p.x + horpomeraj * 15, k2p.y + verpomeraj * 3 + verpomeraj / 2), vec2(horpomeraj / 2, verpomeraj / 2), Kretanje::Horizontalno, 100.0f, horpomeraj * 13, true, -1, 9.0f * horpomeraj);
		k2->dodajObjekat(mrav4);

		//crvici
		for (int i = 5; i < 14; i++) {
			if (i % 2 == 0)
				k2->dodajObjekat(new Neprijatelj(vec2(k2p.x + horpomeraj * i + horpomeraj * 0.5f, k2p.y + verpomeraj * 8), vec2(horpomeraj * 0.1f, verpomeraj), Kretanje::Vertikalno, 90.0f, verpomeraj * 3.5f, true, -1));
			else
				k2->dodajObjekat(new Neprijatelj(vec2(k2p.x + horpomeraj * i + horpomeraj * 0.33f, k2p.y + verpomeraj * 4 + 5.0f), vec2(horpomeraj * 0.3f, verpomeraj), Kretanje::Vertikalno, 70.0f, verpomeraj * 3.5f-5.0f, true));
		}

		//brzi ljuti crveni mrav
		Neprijatelj* crveniMrav = new Neprijatelj(vec2(k2p.x+horpomeraj*14+horpomeraj/8,k2p.y+verpomeraj*10+verpomeraj/2),vec2(horpomeraj/2+horpomeraj/4, verpomeraj / 2), Kretanje::Horizontalno, 433.0f, horpomeraj * 11, false, -1);
		k2->dodajObjekat(crveniMrav);

	// 
	//! 
	//! 
	//! prvo for loop za zidove i platforme
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 12; j++) {
			if (i == 0 && j != 8 || j==0 || i==15 || j==11&&i!=1)
				k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj * i, k2p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
			if((j==2||j==7||j==8) && i!=1 && i!=14 && i!=0 && i!=15)
				k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj * i, k2p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
			if ((j == 4 || j == 5) && i != 1 && i != 3 && i != 0 && i!=15)
				k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj * i, k2p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
		}
	}
		//popunjavam rupe u zidovima i podu koje nisu obuhvacene u for loopu...
		k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj, k2p.y + verpomeraj*9), vec2(horpomeraj, verpomeraj)));
		k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj*2, k2p.y + verpomeraj*9), vec2(horpomeraj, verpomeraj)));
		k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj * 2, k2p.y + verpomeraj * 3), vec2(horpomeraj, verpomeraj)));
		k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj * 2, k2p.y + verpomeraj * 6), vec2(horpomeraj, verpomeraj)));

		//pomerajuca platforma/lift na pocetku kvadranta
		k2->dodajObjekat(new Platforma(vec2(k2p.x + horpomeraj, k2p.y + verpomeraj * 9),vec2(horpomeraj,verpomeraj*0.5f),Kretanje::Vertikalno,100.0f,verpomeraj*7+verpomeraj/2,-1,true));

		//merdevine u redu br j==7
		k2->dodajObjekat(new Merdevine(vec2(k2p.x + horpomeraj * 14 + 5.0f, k2p.y + verpomeraj * 7), vec2(sk / 20.0f, vk / 6.0f)));
	
		//merdevine i njihove platforme u redu 10
		Platforma* prveMerdevPlatf = new Platforma(vec2(k2p.x + horpomeraj * 14,k2p.y+ verpomeraj * 10), vec2(horpomeraj, vk / 32.0f));
		prveMerdevPlatf->vrhMerdevina(true);
		Merdevine* prveMerdev2k = new Merdevine(vec2(k2p.x+ horpomeraj * 14 + 5.0f,k2p.y+ verpomeraj * 10), vec2(sk / 20.0f, verpomeraj));
	
		Platforma* drugeMerdevPlatf = new Platforma(vec2(k2p.x + horpomeraj * 10, k2p.y + verpomeraj * 10), vec2(horpomeraj, vk / 32.0f));
		drugeMerdevPlatf->vrhMerdevina(true);
		Merdevine* drugeMerdev2k = new Merdevine(vec2(k2p.x + horpomeraj * 10 + 5.0f, k2p.y + verpomeraj * 10), vec2(sk / 20.0f, verpomeraj));

		Platforma* treceMerdevPlatf = new Platforma(vec2(k2p.x + horpomeraj * 6, k2p.y + verpomeraj * 10), vec2(horpomeraj, vk / 32.0f));
		treceMerdevPlatf->vrhMerdevina(true);
		Merdevine* treceMerdev2k = new Merdevine(vec2(k2p.x + horpomeraj * 6 + 5.0f, k2p.y + verpomeraj * 10), vec2(sk / 20.0f, verpomeraj));

	

		//prvo dodajem merdevine...
		k2->dodajObjekat(prveMerdev2k);
		k2->dodajObjekat(drugeMerdev2k);
		k2->dodajObjekat(treceMerdev2k);

		//pa onda i platforme
		k2->dodajObjekat(prveMerdevPlatf);
		k2->dodajObjekat(drugeMerdevPlatf);
		k2->dodajObjekat(treceMerdevPlatf);

		//portal/cevka za kraj drugog i pocetak treceg kvadranta!
		Portal* portal1 = new Portal(vec2(k2p.x+horpomeraj,k2p.y+verpomeraj*11+verpomeraj*0.66f),vec2(horpomeraj,verpomeraj/3.0f),vec2(k3p.x+horpomeraj*14+horpomeraj/4.0f,k3p.y+verpomeraj*10));
		k2->dodajObjekat(portal1);

		//cilj/sir drugi kvadrant
		Cilj* sir2 = new Cilj(vec2(k2p.x + horpomeraj * 2 +22.0f, k2p.y + verpomeraj * 10+16.0f),vec2(16.0f,16.0f));
		k2->dodajObjekat(sir2);
		k2->setujCilj(sir2);

	//!!!TRECI KVADRANT OBJEKTI!!!
	//! 
	//! 
		//sada vec standardni for loop za platforme/zidove...
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 12; j++) {
				if (j==0 && i!=1 || j==1 || j==11)
					k3->dodajObjekat(new Platforma(vec2(k3p.x + horpomeraj * i, k3p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if ((i==0||i==15) && j>2 && j<11)
					k3->dodajObjekat(new Platforma(vec2(k3p.x + horpomeraj * i, k3p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if ((i == 4 || i == 9) && j>1 && j<10)
					k3->dodajObjekat(new Platforma(vec2(k3p.x + horpomeraj * i+horpomeraj/4, k3p.y + verpomeraj * j), vec2(horpomeraj/2, verpomeraj)));
				if((j==9 || j==10)&&(i==6 || i==7 || i== 11 || i==13))
					k3->dodajObjekat(new Platforma(vec2(k3p.x + horpomeraj * i, k3p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));

				//stalagmiti... kao neprijatelji!
				if ((j == 9 && (i == 1 || i == 2 || i == 12)) || (j==7 && (i==6 || i==7 || i==11 || i==13)))
					k3->dodajObjekat(new Neprijatelj(vec2(k3p.x + horpomeraj * i+horpomeraj/8, k3p.y + verpomeraj * j),vec2(horpomeraj/2+horpomeraj/4,verpomeraj*2.0f),Kretanje::Horizontalno,0.0f,0.0f));
				
				//a gde su stalagmiti, tu su i stalaktiti! (OPASNOST! OTROVNE KAPLJICE!)
				if (j == 2 && (i == 2 || i == 11 || i == 13)) {
					k3->dodajObjekat(new Neprijatelj(vec2(k3p.x + horpomeraj * i + horpomeraj / 8, k3p.y + verpomeraj * j), vec2(horpomeraj / 2 + horpomeraj / 4, verpomeraj * 2.0f), Kretanje::Horizontalno, 0.0f, 0.0f));
					if(i!=2)
						k3->dodajObjekat(new Neprijatelj(vec2(k3p.x + horpomeraj * i + horpomeraj / 2 -6.0f, k3p.y + verpomeraj*4), vec2(12.0f, 12.0f), Kretanje::Vertikalno,70.0f,4*verpomeraj,true));

				}

				//merdevine...
				if ((i == 5 || i == 12) && j == 4)
					k3->dodajObjekat(new Merdevine(vec2(k3p.x + horpomeraj * i +5.0f, k3p.y + verpomeraj * j), vec2(sk / 20.0f, verpomeraj * 3)));
				if(j==4 && (i==3 || i==8 || i==10 || i==14))
					k3->dodajObjekat(new Merdevine(vec2(k3p.x + horpomeraj * i + 5.0f, k3p.y + verpomeraj * j), vec2(sk / 20.0f, verpomeraj * 7)));
				Merdevine* merdevKraj3k = new Merdevine(vec2(k3p.x + horpomeraj + 5.0f, k3p.y + verpomeraj * 2), vec2(sk / 20.0f, verpomeraj * 7));
				k3->dodajObjekat(merdevKraj3k);
			}

			//neprijatelji 3. kvadranta, slepi misevi! Ovi piju krv!
			Neprijatelj* velikiSismis1 = new Neprijatelj(vec2(k3p.x+horpomeraj*6+5.0f,k3p.y+verpomeraj*2),vec2(horpomeraj-10.0f,verpomeraj-25.0f),Kretanje::Vertikalno,70.0f,4*verpomeraj+25.0f,false);
			Neprijatelj* velikiSismis2 = new Neprijatelj(vec2(k3p.x + horpomeraj * 7 + 5.0f, k3p.y + verpomeraj * 6 +25.0f), vec2(horpomeraj - 10.0f, verpomeraj - 25.0f), Kretanje::Vertikalno, 70.0f, 4 * verpomeraj+25.0f, false, -1);
			k3->dodajObjekat(velikiSismis1);
			k3->dodajObjekat(velikiSismis2);
			Neprijatelj* brziSismis1 = new Neprijatelj(vec2(k3p.x+horpomeraj+horpomeraj*0.25f,k3p.y+verpomeraj*2),vec2(horpomeraj/2,verpomeraj*0.66f),Kretanje::Vertikalno,150.0f,6*verpomeraj,false);
			Neprijatelj* brziSismis2 = new Neprijatelj(vec2(k3p.x + horpomeraj*3 + horpomeraj * 0.25f, k3p.y + verpomeraj * 8), vec2(horpomeraj / 2, verpomeraj * 0.66f), Kretanje::Vertikalno, 150.0f, 6 * verpomeraj, false,-1);
			k3->dodajObjekat(brziSismis1);
			k3->dodajObjekat(brziSismis2);

			//dopuna jedne rupe u zidu skroz desno i jedne male merdevinice za game balans..
			k3->dodajObjekat(new Platforma(vec2(k3p.x + horpomeraj * 15, k3p.y + verpomeraj * 2), vec2(horpomeraj, verpomeraj)));
			k3->dodajObjekat(new Merdevine(vec2(k3p.x + horpomeraj * 8 + 5.0f, k3p.y + verpomeraj * 3), vec2(sk / 20.0f, verpomeraj)));

			//i za kraj sir treceg kvadranta.. bravo ako ste stigli dovde!
			Cilj* sir3 = new Cilj(vec2(k3p.x + 22.0f, k3p.y + verpomeraj * 2 + 16.0f), vec2(16.0f, 16.0f));
			k2->dodajObjekat(sir3);
			k2->setujCilj(sir3);

		}



	//!!!CETVRTI KVADRANT OBJEKTI!!!:
		//! 
		//zidovi... samo cele platforme, znaci velicine horpomerajxverpomeraj, vredi naglasiti jer imam poluplatforme u ovom kvadrantu

		for(int i=0;i<16;i++)
			for (int j = 0; j < 12; j++) {
				if (i == 0 && j > 4 || i>0 && j==11)
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj,verpomeraj)));
				if(j==0 && (i<3 || i>10))
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if(j==2 && (i<3 || i==11))
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if(j==4 && i!=6 && i!=11)
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if(j==9 && i>0 && i<8)
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if(j== 8 && i>4 && i<8)
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if((j==6 || j==7) && (i==2 || i==3))
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));
				if(i==9 && (j==5 || j==8 || j==9))
					k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * i, k4p.y + verpomeraj * j), vec2(horpomeraj, verpomeraj)));

			}
			//popunjavam rupe, tacnije 2 pune u 2. redu
			k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * 2.0f, k4p.y + verpomeraj), vec2(horpomeraj, verpomeraj)));
			k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * 11.0f, k4p.y + verpomeraj), vec2(horpomeraj, verpomeraj)));

			//jedna tanja platforma kod ulaza..
			k4->dodajObjekat(new Platforma(vec2(k4p.x + horpomeraj * 15.0f, k4p.y + verpomeraj * 3.0f), vec2(horpomeraj, verpomeraj / 3.0f)));

			//poluplatforme, izgledace kao cevi
			//prvo dve najtanje na vrhu..
			Platforma* cevTanka1 = new Platforma(vec2(k4p.x+horpomeraj*4.0f,k4p.y+verpomeraj),vec2(horpomeraj*6.0f,verpomeraj/4.0f));
			Platforma* cevTanka2 = new Platforma(vec2(k4p.x + horpomeraj * 3.0f, k4p.y + verpomeraj*2.0f + verpomeraj *0.75f), vec2(horpomeraj * 6.0f, verpomeraj / 4.0f));
			k4->dodajObjekat(cevTanka1);
			k4->dodajObjekat(cevTanka2);
			//pa onda jedna malo nize..
			Platforma* cevTanka3 = new Platforma(vec2(k4p.x + horpomeraj * 4.0f, k4p.y + verpomeraj * 7.0f - verpomeraj/8.0f), vec2(horpomeraj * 6.0f, verpomeraj / 4.0f));
			k4->dodajObjekat(cevTanka3);
			//pa jedna kraca i deblja izmedju njih..
			Platforma* cevSrednja = new Platforma(vec2(k4p.x + horpomeraj * 5.0f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj * 3.0f, verpomeraj / 2.0f));
			k4->dodajObjekat(cevSrednja);
			//i tri vertikalne..
			Platforma* cevVert1 = new Platforma(vec2(k4p.x + horpomeraj * 10.0f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj / 2.0f, verpomeraj * 6.0f));
			Platforma* cevVert2 = new Platforma(vec2(k4p.x + horpomeraj * 12.0f+horpomeraj*0.375f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj / 4.0f, verpomeraj * 6.0f));
			Platforma* cevVert3 = new Platforma(vec2(k4p.x + horpomeraj * 14.0f+ horpomeraj * 0.375f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj / 4.0f, verpomeraj * 6.0f));
			k4->dodajObjekat(cevVert1);
			k4->dodajObjekat(cevVert2);
			k4->dodajObjekat(cevVert3);

			//e sad merdevine koje ce izgledati kao kanapi..
			//prvo najduzi kanapi, desno gde su vertikalne cevi
			Merdevine* dugiKanap1 = new Merdevine(vec2(k4p.x+horpomeraj*11.0f+horpomeraj*0.375f,k4p.y+verpomeraj*5.0f),vec2(horpomeraj/4.0f,verpomeraj*6.0f));
			Merdevine* dugiKanap2 = new Merdevine(vec2(k4p.x + horpomeraj * 13.0f + horpomeraj * 0.375f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj / 4.0f, verpomeraj * 6.0f));
			Merdevine* dugiKanap3 = new Merdevine(vec2(k4p.x + horpomeraj * 15.0f + horpomeraj * 0.375f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj / 4.0f, verpomeraj * 6.0f));
			k4->dodajObjekat(dugiKanap1);
			k4->dodajObjekat(dugiKanap2);
			k4->dodajObjekat(dugiKanap3);

			//pa 2 kanapa srednje duzine
			Merdevine* srednjiKanap1 = new Merdevine(vec2(k4p.x+horpomeraj+horpomeraj*0.375f,k4p.y+verpomeraj*5.0f),vec2(horpomeraj/4.0f,verpomeraj*4.0f));
			Merdevine* srednjiKanap2 = new Merdevine(vec2(k4p.x + horpomeraj*8.0f + horpomeraj * 0.375f, k4p.y + verpomeraj * 7.0f+verpomeraj/8.0f), vec2(horpomeraj / 4.0f, verpomeraj * 4.0f-verpomeraj/8.0f));
			k4->dodajObjekat(srednjiKanap1);
			k4->dodajObjekat(srednjiKanap2);

			//i za kraj mali kanapcici od vrha ka dole
			Merdevine* maliKanap1 = new Merdevine(vec2(k4p.x+horpomeraj*7.0f,k4p.y),vec2(horpomeraj/4.0f,verpomeraj));
			Merdevine* maliKanap2 = new Merdevine(vec2(k4p.x + horpomeraj * 7.0f, k4p.y+verpomeraj+verpomeraj/4.0f), vec2(horpomeraj / 4.0f, verpomeraj+verpomeraj/8.0f));
			Merdevine* maliKanap3 = new Merdevine(vec2(k4p.x + horpomeraj * 12.0f, k4p.y+verpomeraj), vec2(horpomeraj / 4.0f, verpomeraj*2.0f-verpomeraj/8.0f));
			Merdevine* maliKanap4 = new Merdevine(vec2(k4p.x + horpomeraj * 8.0f, k4p.y+verpomeraj*5.0f), vec2(horpomeraj / 4.0f, verpomeraj*2.0f-verpomeraj/8.0f));
			Merdevine* maliKanap5 = new Merdevine(vec2(k4p.x + horpomeraj * 4.0f, k4p.y + verpomeraj*5.0f), vec2(horpomeraj / 4.0f, verpomeraj * 2.0f - verpomeraj / 8.0f));
			Merdevine* maliKanap6 = new Merdevine(vec2(k4p.x + horpomeraj * 4.0f, k4p.y + verpomeraj*7.0f), vec2(horpomeraj / 4.0f, verpomeraj * 2.0f - verpomeraj / 8.0f));


			k4->dodajObjekat(maliKanap1);
			k4->dodajObjekat(maliKanap2);
			k4->dodajObjekat(maliKanap3);
			k4->dodajObjekat(maliKanap4);
			k4->dodajObjekat(maliKanap5);
			k4->dodajObjekat(maliKanap6);

			//I ZA KRAJ PORTALIII! IMA IH SVEGA 7!!! Zovem ih cevka ovde jer ce izgledati kao da igrac udje u cevku i izadje na drugu
			Portal* cevka1 = new Portal(vec2(k4p.x+horpomeraj*15.0f,k4p.y+verpomeraj*2.0f),vec2(horpomeraj/4.0f,verpomeraj),vec2(k4p.x+horpomeraj*12.0f+horpomeraj/4.0f, k4p.y+verpomeraj+verpomeraj/4.0f));
			Portal* cevka2 = new Portal(vec2(k4p.x + horpomeraj * 11.0f, k4p.y + verpomeraj * 4.0f), vec2(horpomeraj, verpomeraj/4.0f), vec2(k4p.x+horpomeraj * 11.0f + horpomeraj / 4.0f, k4p.y+verpomeraj*5.0f + verpomeraj / 4.0f));
			Portal* cevka3 = new Portal(vec2(k4p.x + horpomeraj * 11.0f, k4p.y + verpomeraj * 10.0f + verpomeraj*0.75f), vec2(horpomeraj, verpomeraj/4.0f), vec2(k4p.x+horpomeraj * 15.0f + horpomeraj / 4.0f, k4p.y+verpomeraj * 10.0f - verpomeraj / 4.0f));
			Portal* cevka4 = new Portal(vec2(k4p.x + horpomeraj * 15.0f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj, verpomeraj/4.0f), vec2(k4p.x+ horpomeraj * 13.0f + horpomeraj / 4.0f, k4p.y+ verpomeraj *5.0f + verpomeraj / 4.0f));
			Portal* cevka5 = new Portal(vec2(k4p.x + horpomeraj * 13.0f, k4p.y + verpomeraj * 10.0f + verpomeraj*0.75f), vec2(horpomeraj, verpomeraj/4.0f), vec2(k4p.x+ horpomeraj * 9.0f - horpomeraj / 4.0f, k4p.y+ verpomeraj / 4.0f));
			Portal* cevka6 = new Portal(vec2(k4p.x + horpomeraj * 6.0f, k4p.y + verpomeraj * 4.0f+verpomeraj*0.75f), vec2(horpomeraj, verpomeraj/4.0f), vec2(k4p.x+ horpomeraj + horpomeraj / 4.0f, k4p.y+ verpomeraj*10.0f + verpomeraj / 4.0f));
			Portal* cevka7 = new Portal(vec2(k4p.x + horpomeraj * 8.0f, k4p.y + verpomeraj * 5.0f), vec2(horpomeraj, verpomeraj/4.0f), vec2(k4p.x +horpomeraj + horpomeraj / 4.0f, k4p.y+ verpomeraj + verpomeraj / 4.0f));

			k4->dodajObjekat(cevka1);
			k4->dodajObjekat(cevka2);
			k4->dodajObjekat(cevka3);
			k4->dodajObjekat(cevka4);
			k4->dodajObjekat(cevka5);
			k4->dodajObjekat(cevka6);
			k4->dodajObjekat(cevka7);

			//POSLEDNJI sir!!! Najbolje za kraj!
			Cilj* sir4 = new Cilj(vec2(k4p.x+22.0f,k4p.y+verpomeraj+16.0f),vec2(16.0f,16.0f));
			k4->dodajObjekat(sir4);
			k4->setujCilj(sir4);

	//KONACNO DODAVANJE KVADRANTA NA NIVO
	kvadranti.push_back(k1);
	kvadranti.push_back(k2);
	kvadranti.push_back(k3);
	kvadranti.push_back(k4);
}