#pragma once
#include "Objekat.h"

class InputHendler;
class Renderer;

class Igrac : public Objekat {
public:
	Igrac(glm::vec2 poz, glm::vec2 vel);

	void procesInputa(InputHendler* inputi);
	void update(float deltaTime) override;
	void render(Renderer& renderer) override;
	void kolizija(Igrac* igrac) override {};
	void reset(glm::vec2 pocPozicija);
	bool jelNaZemlji() const;
	bool jelNaMerdevinama() const;
	void setNaZemlji(bool vrednost);
	void setNaMerdevinama(bool vrednost);
private:
	glm::vec2 ubrzanje;
	glm::vec2 pocPozicija;
	bool naZemlji;
	bool naMerdevinama;
	static const float brzinaKretanja;
	static const float gravitacija;
	static const float brzinaPenjanja;
};