#include "Merdevine.h"
#include "Igrac.h"
#include "Renderer.h"

Merdevine::Merdevine(vec2 poz, vec2 vel) :Objekat(poz, vel) {

}

void Merdevine::update(float deltaTime){}
void Merdevine::render(Renderer& renderer, vec2 pomeraj) {
	renderer.drawSprite(renderer.getUmetnuTeksturu(), poz+pomeraj, vel);
}
void Merdevine::kolizija(Igrac* igrac) {
	igrac->setNaMerdevinama(true);
}