#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <string>
#include "Texture.h"
using namespace glm;



class Renderer {

public:
	Renderer(int sirinaProz, int visinaProz);
	~Renderer();

	void clear();
	void drawSprite(Texture& texture, vec2 poz, vec2 vel);
	void crtajLinije(int sirinaProz, int visinaProz); //20.06. crta linije izmedju kvadranta, vizuelna pomoc za dev
	Texture& getUmetnuTeksturu();
private:
	int sirinaProz;
	int visinaProz;
	GLuint VAO;
	GLuint VBO;
	GLuint shaderProgram;
	Texture umetnaTekstura;

	void initShaders();
	void initBuffers();
	

	GLuint compileShader(const std::string& source, GLenum type);
};