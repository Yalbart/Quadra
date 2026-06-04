#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

Texture::Texture() {
	id = 0;
	sirina = 0;
	visina = 0;
}

Texture::~Texture() {
	if (id != 0) {
		glDeleteTextures(1, &id);
		//ako id postoji, brisi teksturu
	}
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::load(const std::string& filepath) {
	stbi_set_flip_vertically_on_load(true);
	//neophodno, inace se slika crta naopacke
	int kanali;
	unsigned char* data = stbi_load(filepath.c_str(), &sirina, &visina, &kanali, 4);

	if (!data) {
		std::cout<<"Neuspesno load-ovana tekstura, nesto ne radi :(" << filepath << " - " << stbi_failure_reason() << std::endl;
		return;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sirina, visina, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}