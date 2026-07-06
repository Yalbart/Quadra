#pragma once
#include "Objekat.h"
#include "Kretanje.h"
class Renderer;
enum class TipPlatforme {
	staticna,dinamicna
};

class Platforma : public Objekat {
public:
	Platforma(vec2 poz, vec2 vel);
	Platforma(vec2 poz, vec2 vel, Kretanje kretanje,float brzina, float opseg);//dodaj kretanje kao zaseban header

	void update(float deltaTime) override;
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override;
	void vrhMerdevina(bool v);
private:
	TipPlatforme tip;
	Kretanje kretanje;
	float brzina;
	float opseg;
	float predjenaDistanca;
	int smer;
	bool jelVrhMerdevina = false;

};