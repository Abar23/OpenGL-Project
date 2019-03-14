#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GL\glew.h"
#include "glm\glm.hpp"
#include "Shader.h"
#include "Texture.h"

struct MeshVertex
{
	glm::vec3 positions;
	glm::vec3 normals;
	glm::vec2 texCoords;
};

struct MeshTexture
{
	std::string type;
	std::string path;
	Texture *texture;
};

class Mesh
{
public:
	// Mesh buffer data
	std::vector<MeshVertex> vertices;
	std::vector<MeshTexture> textures;
	std::vector<unsigned int> indices;

	Mesh(std::vector<MeshVertex> vertices, std::vector<MeshTexture> textures, std::vector<unsigned int> indices, GLuint shaderProgramID);
	~Mesh();

	// Rendering method
	void Draw(Shader *shader);
private:	
	GLuint VAO, EBO, VBO;

	void setupMesh(GLuint shaderProgramID);
};

#endif // MESH_H