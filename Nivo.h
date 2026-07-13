#pragma once
#include "Kvadrant.h"
#include <vector>
#include <functional> 

class Renderer;
class Menadzer;
class InputHendler;
class Igrac;

class Nivo {
public:
	Nivo(Renderer* renderer, Menadzer* menadzerResursa, int sirinaProz, int visinaProz);
	~Nivo();
	void procesInputa(InputHendler* inputi);
	void update(float deltaTime);
	void render();
	bool jelPredjen() const;
	
	//13.07.
	std::function<void()> skupljenSir;

	void jelRipno(std::function<void()> rip);
	void resetSveIgrace();
	
private:
	std::vector<Kvadrant*> kvadranti;
	int trenutniKvadrant;
	int sirinaProz;
	int visinaProz;
	Renderer* renderer;
	Menadzer* resursi;
	Igrac* igrac;
	void ucitajKvadrant();
};