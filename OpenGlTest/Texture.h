#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "GL\glew.h"

using namespace std;

class Texture
{
public:
	GLuint textureID;

	Texture(const char *filePathOfImage);

	~Texture();

	void UnbindTexture();

	void BindTexture(uint8_t textureUnit);
};

#endif //TEXTURE_H