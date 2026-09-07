#pragma once
#include "Objekat.h"

class Renderer;

class Portal : public Objekat {
public:
	Portal(vec2 poz, vec2 vel,vec2 destinacija);
	Portal(vec2 poz, vec2 vel, vec2 destinacija, Menadzer* resursi, const std::string& putanjaTeksture);

	void update(float deltaTime) override {}
	void render(Renderer& renderer) override ;
	void kolizija(Igrac* igrac) override ;

private:
	vec2 destinacija;
};