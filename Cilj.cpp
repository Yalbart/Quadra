#include "Cilj.h"

Cilj::Cilj(vec2 poz, vec2 vel) : Objekat(poz, vel) {

}
//void Cilj::update(float deltaTime);
void Cilj::render(Renderer& renderer) {
	renderer.drawSprite(renderer.getUmetnuTeksturu(), poz, vel);
}
void Cilj::kolizija(Igrac* igrac) {
	this->stigao = true;
	this->stigaoDoKraja();
}

