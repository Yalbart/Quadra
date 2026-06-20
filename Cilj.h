#pragma once
#include "Objekat.h"
#include "Renderer.h"
#include "Igrac.h"

class Cilj : public Objekat {
public:
	Cilj(vec2 poz, vec2 vel);
	void update(float deltaTime) override {};
	void render(Renderer& renderer, vec2 pomeraj) override;
	void kolizija(Igrac* igrac) override;
	bool stigaoDoKraja() const { return stigao; }; //const i necu ga pisati u cpp, 
private:
	bool stigao = false;
};