#pragma once
#include "Objekat.h"
class Renderer;
class Merdevine : public Objekat {
public:
	Merdevine(vec2 poz, vec2 vel);
	Merdevine(vec2 poz, vec2 vel, Menadzer* resursi, const std::string& putanjaTeksture);

	void update(float deltaTime) override;
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override;
};