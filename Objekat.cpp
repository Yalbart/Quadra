#include "Objekat.h"

Objekat::Objekat(vec2 poz, vec2 vel)
{
	this->poz = poz;
	this->vel = vel;
	this->tekstura = nullptr;
}

// mogao sam i da koristim listu inicijalizacija clanova (dvotacka pre tela konstruktora)
// ali se ovako lakse cita pa cu ovako koristiti inicijalizaciju