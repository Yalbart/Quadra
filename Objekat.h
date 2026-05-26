#pragma once
#include <glm.hpp>
using namespace glm;

class Renderer; // ovo koristim kao deklaraciju kako ne bih morao da include celu klasu, jer mi ne treba
				// treba mi samo da kompajler zna da ovo postoji jer koristim referencu na tip Renderer
class Igrac;
class Objekat {
public: 
	vec2 poz;
	vec2 vel;
	Objekat(vec2 poz, vec2 vel);
	virtual ~Objekat() = default; //ovde koristim default umesto samo praznog destruktora, to je praksa kad su igrice u pitanju, nije toliko bitno ovde ali potencijalno sprecava neke probleme kasnije
							 // osim toga, destruktor je virtual kako pozivanje na brisanje klasa
							 // koje nasledjuju Objekat sa "delete" ne bi dovelo do curenja memorije
	virtual void update(float deltaTime) = 0;
	virtual void render(Renderer& renderer) = 0; // virtual metode koje su =0 cini klasu abstraktnom
			// ovo je tako jer necemo koristiti objekat klasu direktno, samo klase koje ga nasledjuju
	virtual void kolizija(Igrac* igrac) {}
};