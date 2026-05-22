#pragma once
#include "Kvadrant.h"
#include <vector>

class Renderer;
class Menadzer;
class InputHendler;

class Nivo {
public:
	Nivo(Renderer* renderer, Menadzer* menadzerResursa);
	~Nivo();
	void procesInputa(InputHendler* inputi);
	void update(float deltaTime);
	void render();
	bool jelPredjen() const;
private:
	std::vector<Kvadrant*> kvadranti;
	int trenutniKvadrant;
	Renderer* renderer;
	Menadzer* resursi;
	void ucitajKvadrant();
};