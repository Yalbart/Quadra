#include"InputHendler.h"

InputHendler::InputHendler() {

}

void InputHendler::gore(unsigned char dugme) {
	dugmici[dugme] = false;
}

void InputHendler::dole(unsigned char dugme) {
	dugmici[dugme] = true;
}

void InputHendler::specGore(int dugme) {
	specDugmici[dugme] = false;
}

void InputHendler::specDole(int dugme) {
	specDugmici[dugme] = true;
}

bool InputHendler::jelDole(unsigned char dugme) const {
	auto i = dugmici.find(dugme);
	//koriscenjem find() umesto samo indeksovanja tj [] specava da se upisuju beskorisne false vrednosti non stop
	return i != dugmici.end() && i->second;
}

bool InputHendler::jelSpecDole(int dugme) const {
	auto i = specDugmici.find(dugme);
	return i != specDugmici.end() && i->second;
}

//metode su podeljene u obicne i specijalne zbog freeglut-a, on gleda na keyeve tj dugmice kao obicne i specijalne
//specijalni dugmici bi bili f1-f12, home, insert delete i slicno, sve u ovom fajlu je napisano za 