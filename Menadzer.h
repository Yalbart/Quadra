#pragma once
#include "Texture.h"
#include <string>
#include <unordered_map>
using namespace std;

class Menadzer {

public: 
	Menadzer();
	~Menadzer();
	Texture& getTexture(const string& filepath);
	void clear();
private:
	unordered_map<string, Texture*> teksture;

};