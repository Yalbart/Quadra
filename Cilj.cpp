#include "Cilj.h"

Cilj::Cilj(vec2 poz, vec2 vel) :  Objekat(poz, vel) {

}
//void Cilj::update(float deltaTime);
void Cilj::render(Renderer& renderer, vec2 pomeraj) {
	renderer.drawSprite(renderer.getUmetnuTeksturu(), poz+pomeraj, vel,vec3(0.0f,1.0f,0.0f));
}
void Cilj::kolizija(Igrac* igrac) {
	this->stigao = true;
	this->stigaoDoKraja();
}

