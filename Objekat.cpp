#include "Objekat.h"

Objekat::Objekat(vec2 poz, vec2 vel)
{
	this->poz = poz;
	this->vel = vel;
	this->tekstura = nullptr;
}

Objekat::Objekat(vec2 poz, vec2 vel, Menadzer* resursi, const std::string& putanjaTeksture) : poz(poz), vel(vel), oznacenZaBrisanje(false) {
	tekstura = &resursi->getTexture(putanjaTeksture);
}