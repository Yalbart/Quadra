#include "Platforma.h"
#include "Igrac.h"
#include "Renderer.h"
#include <algorithm>
#include <iostream>

//staticna platforma , njen konstruktor
Platforma::Platforma(vec2 poz,vec2 vel): Objekat(poz,vel), tip(TipPlatforme::staticna), kretanje(Kretanje::Horizontalno), resetPoz(false){
	this->brzina = 0.0f;
	this->opseg = 0.0f;
	this->predjenaDistanca = 0.0f;
	this->smer = 1;
	this->pocPoz = poz;
}

//07/09/2026 const za teksturu staticnu
Platforma::Platforma(vec2 poz, vec2 vel, Menadzer* resursi, const std::string& putanjaTeksture) : Objekat(poz, vel), tip(TipPlatforme::staticna), kretanje(Kretanje::Horizontalno), resetPoz(false) {
	this->brzina = 0.0f;
	this->opseg = 0.0f;
	this->predjenaDistanca = 0.0f;
	this->smer = 1;
	this->pocPoz = poz; 
	tekstura = &resursi->getTexture(putanjaTeksture);
}

//dinamicna platforma, njen konstruktor
Platforma::Platforma(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg,int smer, bool resetPoz) : Objekat(poz,vel),tip(TipPlatforme::dinamicna),kretanje(kretanje){
	this->brzina = brzina;
	this->opseg = opseg;
	this->predjenaDistanca = 0.0f;
	this->smer = smer;
	this->pocPoz = poz;
	this->resetPoz = resetPoz;
}

void Platforma::update(float deltaTime) {
	if (tip == TipPlatforme::staticna) return;
	float pokret = brzina * smer * deltaTime;
	//std::cout << "brzina: " << brzina << " opseg: " << opseg << " smer: " << smer << " distanca: " << predjenaDistanca << std::endl;
	if (kretanje == Kretanje::Horizontalno)
		poz.x += pokret;
	else
		poz.y += pokret;

	predjenaDistanca += brzina * deltaTime;

	if(predjenaDistanca >= opseg)
	{
		predjenaDistanca = 0;
		if (resetPoz == true)
			poz = pocPoz;
		else
			smer *= -1;
	}
}

void Platforma::render(Renderer& renderer) {
	Texture& tex = tekstura ? *tekstura : renderer.getUmetnuTeksturu();
	renderer.drawSprite(tex, poz, vel,vec3(0.5f,0.5f,0.5f));
}

void Platforma::kolizija(Igrac* igrac) {
	//std::cout << "KOLIZIJA DETEKTOVANA" << std::endl;
	if (igrac->jelNaMerdevinama() && jelVrhMerdevina == true)
		return; //ako je igrac na merdevinama i takodje je na vrhu merdevina, preskoci koliziju sa platformom

	float igracDonja = igrac->poz.y + igrac->vel.y;
	float igracGornja = igrac->poz.y;
	float igracDesna = igrac->poz.x + igrac->vel.x;
	float igracLeva = igrac->poz.x;
	float platfDonja = poz.y + vel.y;
	float platfGornja = poz.y;
	float platfDesna = poz.x + vel.x;
	float platfLeva = poz.x;

	float preklapanjeGore = igracDonja - platfGornja;
	float preklapanjeDole = platfDonja - igracGornja;
	float preklapanjeLevo = igracDesna - platfLeva;
	float preklapanjeDesno = platfDesna - igracLeva;
	float minPreklapanje = std::min({ preklapanjeGore,preklapanjeDole,preklapanjeLevo,preklapanjeDesno });

	if (minPreklapanje == preklapanjeGore) {
		//igrac stoji na platformi
		igrac->poz.y = platfGornja - igrac->vel.y;
		igrac->setUbrzanjeY(0.0f);
		igrac->setNaZemlji(true);
	}
	else if (minPreklapanje == preklapanjeDole) {
		//igrac je udario platformu od dole
		igrac->poz.y = platfDonja;
		igrac->setUbrzanjeY(0.0f);
	}
	else if (minPreklapanje == preklapanjeLevo) {
		//igrac je udario u levu ivicu
		igrac->poz.x = platfLeva - igrac->vel.x;
	}
	else {
		//igrac je udario u desnu ivicu platforme/bloka
		igrac->poz.x = platfDesna;
	}
}

void Platforma::vrhMerdevina(bool v) {
	jelVrhMerdevina = v;
}