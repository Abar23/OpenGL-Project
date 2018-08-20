#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "GL\glew.h"

using namespace std;

class Texture
{
public:
	Texture(const char *filePathOfImage);

	~Texture();

	void UnbindTexture();

	void BindTexture(uint8_t textureUnit);

private:
	GLuint textureID;
};

#endif //TEXTURE_H