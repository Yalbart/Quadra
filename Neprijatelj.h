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
	Neprijatelj(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg,bool resetPoz=false, int smer=1,float pocDistanca=0);
	//note za neprijatelje 08.09.2026. tekstura mora da se umece na odredjen nacin, znaci nakon opsega se navode i prosledjuju resursi, pa putanja, pa onda ostatak konstruktora, jer default values moraju biti na kraju konstruktora
	Neprijatelj(vec2 poz, vec2 vel, Kretanje kretanje, float brzina, float opseg, Menadzer* resursi, const std::string& putanjaTeksture, bool resetPoz = false, int smer = 1, float pocDistanca = 0);

	void update(float deltaTime) override;
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override;
private:
	Kretanje kretanje;
	bool resetPoz;
	float brzina;
	float opseg;
	float predjenaDistanca;
	float pocDistanca;
	int smer;
	vec2 pocPozicija;
};