#include "Portal.h"
#include "Igrac.h"
#include "Renderer.h"

Portal::Portal(vec2 poz,vec2 vel,vec2 destinacija):Objekat(poz,vel){
	this->destinacija = destinacija;
}

void Portal::render(Renderer& renderer) {
	Texture& tex = tekstura ? *tekstura : renderer.getUmetnuTeksturu();
	renderer.drawSprite(tex, poz, vel, vec3(0.0, 0.5, 1.0));
}

void Portal::kolizija(Igrac* igrac) {
	igrac->setPoz(destinacija);
	igrac->resetUbrzanje();
}