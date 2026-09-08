#include "Neprijatelj.h"
#include "Igrac.h"
#include "Renderer.h"

Neprijatelj::Neprijatelj(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg,bool resetPoz,int smer,float pocDistanca) : Objekat(poz,vel), kretanje(kretanje) {
	this->brzina = brzina;
	this->opseg = opseg;
	this->predjenaDistanca = pocDistanca;
	this->smer = smer;
	this->pocPozicija = poz;
	this->resetPoz = resetPoz;
}

//note za neprijatelje 08.09.2026. tekstura mora da se umece na odredjen nacin, znaci nakon opsega se navode i prosledjuju resursi, pa putanja, pa onda ostatak konstruktora, jer default values moraju biti na kraju konstruktora
Neprijatelj::Neprijatelj(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg, Menadzer* resursi, const std::string& putanjaTeksture, bool resetPoz, int smer, float pocDistanca) : Objekat(poz, vel), kretanje(kretanje) {
	this->brzina = brzina;
	this->opseg = opseg;
	this->predjenaDistanca = pocDistanca;
	this->smer = smer;
	this->pocPozicija = poz;
	this->resetPoz = resetPoz;
	tekstura = &resursi->getTexture(putanjaTeksture);
}

void Neprijatelj::update(float deltaTime){
	float pokret = brzina * smer * deltaTime;
	if (kretanje == Kretanje::Horizontalno) 
		poz.x += pokret;
	else 
		poz.y += pokret;
	predjenaDistanca += brzina * deltaTime;
	if (predjenaDistanca >= opseg) {
		predjenaDistanca = 0.0f;
		if (resetPoz)
			poz = pocPozicija;
		else
			smer *= -1;
	}
}
void Neprijatelj::render(Renderer& renderer) {
	Texture& tex = tekstura ? *tekstura : renderer.getUmetnuTeksturu();
	renderer.drawSprite(tex, poz, vel,vec3(1.0f,0.0f,0.0f));
}
void Neprijatelj::kolizija(Igrac* igrac) {
	igrac->setRip(true);
	//igrac->reset(igrac->getPocPozicija());
}