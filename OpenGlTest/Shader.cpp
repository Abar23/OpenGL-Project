#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Shader.h"

void CheckCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

static char* ReadShaderSource(const char *shaderPath)
{
	//open file and make it read only
	char* content;
	FILE *shaderCode = fopen(shaderPath, "r");
	if (shaderCode != NULL)
	{
		//move file pointer to the end of the file and determine the length of the file
		fseek(shaderCode, 0, SEEK_END);
		long fileLength = ftell(shaderCode);
		fseek(shaderCode, 0, SEEK_SET);
		//Create char pointer that will point to the text content of the shader
		content = (char *)calloc(fileLength + 1, sizeof(char));
		if (content != NULL)
		{
			//Read the file
			fread(content, 1, fileLength, shaderCode);
			//Denote end of the string with null character
			content[fileLength + 1] = '\0';
		}
		else
		{
			printf("Error ocurred while reading shader file!");
			cin.get();
			exit(0);
		}
		fclose(shaderCode);
	}
	else
	{
		std::cout << "Shader \"" << shaderPath << "\" does not exist!" << std::endl;
		cin.get();
		exit(0);
	}
	return content;
}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
	//Create and compile vertex shader
	const char *vertexShaderCode = ReadShaderSource(vertexShaderPath);
	this->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertexShaderID, 1, (const GLchar **)&vertexShaderCode, NULL);
	glCompileShader(this->vertexShaderID);
	CheckCompileErrors(this->vertexShaderID, "VERTEX");

	//Create and compile fragment shader
	const char *fragmentShaderCode = ReadShaderSource(fragmentShaderPath);
	this->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->fragmentShaderID, 1, (const GLchar **)&fragmentShaderCode, NULL);
	glCompileShader(this->fragmentShaderID);
	CheckCompileErrors(this->fragmentShaderID, "FRAGMENT");

	//Create shader program and link each shader to the program
	this->programID = glCreateProgram();
	glAttachShader(this->programID, this->vertexShaderID);
	glAttachShader(this->programID, this->fragmentShaderID);
	glLinkProgram(this->programID);
	CheckCompileErrors(this->vertexShaderID, "PROGRAM");

	glDetachShader(this->programID, this->vertexShaderID);
	glDetachShader(this->programID, this->fragmentShaderID);
	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);
}

Shader::~Shader()
{
	//Detach shaders from the program and delete them
	glDeleteProgram(this->programID);
}

void Shader::UseProgram()
{
	glUseProgram(this->programID);
}

void Shader::TurnOffProgram()
{
	glUseProgram(0);
}

GLuint Shader::GetProgramID()
{
	return this->programID;
}

void Shader::SetUniformToTextureUnit(const char *uniformName, uint8_t textureUnit)
{
	this->UseProgram();
	glUniform1i(glGetUniformLocation(this->programID, uniformName), textureUnit);
	this->TurnOffProgram();
}
