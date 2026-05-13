#pragma once
#include <GL/freeglut.h>
#include <unordered_map>
using namespace std;

class InputHendler {
public:
	InputHendler();

	void dole(unsigned char dugme);
	void gore(unsigned char dugme);
	void specDole(int specDugme);
	void specGore(int specDugme);

	bool jelDole(unsigned char dugme)const;
	bool jelSpecDole(int specDugme)const;
private:
	unordered_map<unsigned char, bool> dugmici;
	unordered_map<int, bool> specDugmici;
};