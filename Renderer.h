#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <string>
using namespace glm;

class Texture;

class Renderer {

public:
	Renderer(int sirinaProz, int visinaProz);
	~Renderer();

	void clear();
	void drawSprite(Texture& texture, vec2 poz, vec2 vel);
private:
	int sirinaProz;
	int visinaProz;
	GLuint VAO;
	GLuint VBO;
	GLuint shaderProgram;

	void initShaders();
	void initBuffers();

	GLuint compileShader(const std::string& source, GLenum type);
};