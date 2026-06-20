#pragma once
#include "Objekat.h"
class Renderer;
class Merdevine : public Objekat {
public:
	Merdevine(vec2 poz, vec2 vel);
	void update(float deltaTime) override;
	void render(Renderer& renderer, vec2 pomeraj) override;
	void kolizija(Igrac* igrac) override;
};