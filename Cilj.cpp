#include "Cilj.h"

Cilj::Cilj(vec2 poz, vec2 vel, int* brojac) : Objekat(poz, vel), brojacSira(brojac) {
	stigao = false;
}
//void Cilj::update(float deltaTime);
void Cilj::render(Renderer& renderer) {
	Texture& tex = tekstura ? *tekstura : renderer.getUmetnuTeksturu();
	renderer.drawSprite(tex, poz, vel,vec3(1.0f,1.0f,0.0f));
}
void Cilj::kolizija(Igrac* igrac) {
	if (!stigao) {
		this->stigao = true;
		igrac->setPocPoz(poz);
		if (brojacSira)
			(*brojacSira)++;

		//if (skupljenSirIzIgre)
		//	skupljenSirIzIgre();
		//igra->pojedenSir();
		//jelSkupljenSir = true;
		oznacenZaBrisanje = true;
		std::cout << "Sir pokupljen i obrisan?!" << std::endl;
	}
}

