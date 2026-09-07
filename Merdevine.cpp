#include "Merdevine.h"
#include "Igrac.h"
#include "Renderer.h"

Merdevine::Merdevine(vec2 poz, vec2 vel) :Objekat(poz, vel) {

}
Merdevine::Merdevine(vec2 poz, vec2 vel, Menadzer* resursi, const std::string& putanjaTeksture) :Objekat(poz, vel) {
	tekstura = &resursi->getTexture(putanjaTeksture);
}

void Merdevine::update(float deltaTime){}
void Merdevine::render(Renderer& renderer) {
	Texture& tex = tekstura ? *tekstura : renderer.getUmetnuTeksturu();
	renderer.drawSprite(tex, poz, vel,vec3(0.6f,0.3f,0.1f));
}
void Merdevine::kolizija(Igrac* igrac) {
	igrac->setNaMerdevinama(true);
}