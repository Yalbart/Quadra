#pragma once
#include <GL/glew.h>
#include<string>

class Texture {
public:
	GLuint id;
	int sirina;
	int visina;
	Texture();
	~Texture();
	void load(const std::string& filepath);
	void bind() const;
};