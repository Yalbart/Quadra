#pragma once
#include "Objekat.h"
using namespace glm;
class InputHendler;
class Renderer;

class Igrac : public Objekat {
public:
	Igrac(vec2 poz, vec2 vel);

	void procesInputa(InputHendler* inputi);

	void update(float deltaTime) override;  //ovo su mi override metode iz objekat klase
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override {};

	void reset(vec2 pocPozicija);
	bool jelNaZemlji() const;
	bool jelNaMerdevinama() const;
	void setNaZemlji(bool vrednost);
	void setNaMerdevinama(bool vrednost);
	void setPoz(vec2 novaPoz);
	vec2 getPocPozicija();
	void setUbrzanjeY(float v);
	void resetUbrzanje();
private:
	vec2 ubrzanje;
	vec2 pocPozicija;
	bool naZemlji;
	bool naMerdevinama;
	//ove vrednosti su mi staticne i konstante jer je lakse za podesavanje umesto da koristim neke
	//medzik brojeve i generalno bi to bio spagete kod
	static const float brzinaKretanja;
	static const float gravitacija;
	static const float brzinaPenjanja;
};