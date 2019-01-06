#include "Mesh.h"
#include <iostream>
#include "assimp/Importer.hpp"

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<MeshTexture> textures, std::vector<unsigned int> indices, GLuint shaderProgramID)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	setupMesh(shaderProgramID);
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader *shader)
{
	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
		// Retrieve texture number (the N in texture_diffuseN)
		std::string number;
		std::string name = this->textures[0].type;
		if (name.compare("texture_diffuse"))
		{
			number = std::to_string(diffuseNumber++);
		}
		else 
		{
			number = std::to_string(specularNumber++);
		}

		glUniform1i(glGetUniformLocation(shader->GetProgramID(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// Draw mesh
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh(GLuint shaderProgramID)
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(MeshVertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	// Set vertex position data for the shader in variable with layout of 0
	GLuint attributeID = glGetAttribLocation(shaderProgramID, "inPosition");
	glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)0);
	glEnableVertexAttribArray(attributeID);
	// Set vertex normal data for the shader in variable with layout of 1
	attributeID = glGetAttribLocation(shaderProgramID, "inNormal");
	glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, normals));
	glEnableVertexAttribArray(attributeID);
	// Set texture coordinate data for the in varable with layout of 2
	attributeID = glGetAttribLocation(shaderProgramID, "inTexCoord");
	glVertexAttribPointer(attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, texCoords));
	glEnableVertexAttribArray(attributeID);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
