#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include "GL\glew.h"

using namespace std;

class Shader
{
public:
	Shader(const char *shaderSource, const char *vertexShaderPath);

	~Shader();

	void UseProgram();
	
	void TurnOffProgram();

	GLuint GetProgramID();

	void SetUniformToTextureUnit(const char *uniformName, uint8_t textureUnit);

private:
	GLuint programID;

	GLuint vertexShaderID;

	GLuint fragmentShaderID;
};

#endif //SHADER_H