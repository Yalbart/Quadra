#include "Igrac.h"
#include "Renderer.h"
#include "InputHendler.h"
#include "GL/freeglut.h"

//staticne i konstantne vrednosti i ako zelim da ih podesim podesicu ih ovde u samoj klasi
const float Igrac::brzinaPenjanja = 150;
const float Igrac::brzinaKretanja = 300;
const float Igrac::gravitacija = 600;

Igrac::Igrac(vec2 poz, vec2 vel):Objekat(poz, vel){
	this->ubrzanje.x = 0.0f;
	this->ubrzanje.y = 0.0f;
	this->pocPozicija = poz;
	this->naZemlji = false;
	this->naMerdevinama = false;

}

void Igrac::procesInputa(InputHendler* inputi) {
	//ovo znaci da ce non stop da se prvo stavi ubrzanje na 0, mora tako jer necu klizanje, igrac se zaustavlja u mestu cim nije pritisnuto dugme za kretanje
	ubrzanje.x = 0.0f; 

	if (inputi->jelSpecDole(GLUT_KEY_LEFT))
		ubrzanje.x = -brzinaKretanja;
	if (inputi->jelSpecDole(GLUT_KEY_RIGHT))
		ubrzanje.x = brzinaKretanja;

	if (naMerdevinama) {
		ubrzanje.y = 0.0f;
		if (inputi->jelSpecDole(GLUT_KEY_UP))
			ubrzanje.y = -brzinaPenjanja;
		if (inputi->jelSpecDole(GLUT_KEY_DOWN))
			ubrzanje.y = brzinaPenjanja;
	}
}

void Igrac::update(float deltaTime) {
	if (!naMerdevinama)
		ubrzanje.y += gravitacija * deltaTime; //gravitacija radi samo ako igrac nije na merdevinama

	poz += ubrzanje * deltaTime;


	naMerdevinama = false;
}
//reset nivoa, koristice se kad igrac izgubi
void Igrac::reset(vec2 pocPozicija) {
	poz = pocPozicija;
	ubrzanje = vec2(0.0f, 0.0f);
	naZemlji = false;
	naMerdevinama = false;

}

//obicna provera stanja
bool Igrac::jelNaZemlji() const {
	return naZemlji;
}

bool Igrac::jelNaMerdevinama() const {
	return naMerdevinama;
}

//obicni setteri
void Igrac::setNaZemlji(bool v) {
	naZemlji = v;
}

void Igrac::setNaMerdevinama(bool v) {
	naMerdevinama = v;
}