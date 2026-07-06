#include "Neprijatelj.h"
#include "Igrac.h"
#include "Renderer.h"

Neprijatelj::Neprijatelj(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg,bool resetPoz) : Objekat(poz,vel), kretanje(kretanje) {
	this->brzina = brzina;
	this->opseg = opseg;
	this->predjenaDistanca = 0.0f;
	this->smer = 1;
	this->pocPozicija = poz;
	this->resetPoz = resetPoz;
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
	renderer.drawSprite(renderer.getUmetnuTeksturu(), poz, vel,vec3(1.0f,0.0f,0.0f));
}
void Neprijatelj::kolizija(Igrac* igrac) {
	igrac->reset(igrac->getPocPozicija());
}