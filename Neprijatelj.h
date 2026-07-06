#pragma once
#include "Objekat.h"
#include "Kretanje.h"
class Renderer;

/*enum class Kretanje {
	Horizontalno,
	Vertikalno
};*/

class Neprijatelj : public Objekat {
public:
	Neprijatelj(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg,bool resetPoz=false);

	void update(float deltaTime) override;
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override;
private:
	Kretanje kretanje;
	bool resetPoz;
	float brzina;
	float opseg;
	float predjenaDistanca;
	int smer;
	vec2 pocPozicija;
};