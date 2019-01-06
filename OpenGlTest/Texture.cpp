#include <cassert>
#include <iostream>
#include "Texture.h"
#include "SOIL2\stb_image.h"

Texture::Texture(const char *filePathOfImage)
{
	glGenTextures(1, &this->textureID);

	int width, height, numberOfComponents;
	unsigned char *data = stbi_load(filePathOfImage, &width, &height, &numberOfComponents, 0);
	if (data)
	{
		GLenum format;
		if (numberOfComponents == 1)
			format = GL_RED;
		else if (numberOfComponents == 2)
			format = GL_RG;
		else if (numberOfComponents == 3)
			format = GL_RGB;
		else if (numberOfComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cerr << "Failed to load: " << filePathOfImage << std::endl;
	}

	stbi_image_free(data);
}

void Texture::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::BindTexture(uint8_t textureUnit)
{
	assert(textureUnit >= 0 && textureUnit <= 31);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->textureID);
}
