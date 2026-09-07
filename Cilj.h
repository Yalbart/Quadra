#pragma once
#include "Objekat.h"
#include "Renderer.h"
#include "Igrac.h"
#include <iostream>

class Igra;


class Cilj : public Objekat {
public:
	Cilj(vec2 poz, vec2 vel, int* brojacSira);
	Cilj(vec2 poz, vec2 vel, int* brojacSira, Menadzer* resursi, const std::string& putanjaTeksture);


	void update(float deltaTime) override {};
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override;

	bool stigaoDoKraja() const { return stigao; }; //const i necu ga pisati u cpp,

private:
	
	bool stigao = false;
	int* brojacSira;

};