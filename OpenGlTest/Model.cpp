#include "Model.h"
#include <iostream>
#include <stack>
#include <stdbool.h>
#include "SOIL2\stb_image.h"
#include "glm\glm.hpp"
#include "Mesh.h"
#include "Texture.h"

unsigned int TextureFromFile(const char *path, const string &directory);

Model::Model(std::string const *path, GLuint shaderProgramID)
{
	this->shaderProgramID = shaderProgramID;
	loadModel(path);
}

Model::~Model()
{
}

void Model::Draw(Shader *shader)
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string const *path)
{
	// Read file with ASSIMP
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path->c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// Get the directory path that contains the model file
	this->modelDirectory = path->substr(0, path->find_last_of('/'));

	// Process ASSIMP's root
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *rootNode, const aiScene *scene)
{
	std::stack<aiNode *> childrenNodeOfScene;
	childrenNodeOfScene.push(rootNode);

	while (!childrenNodeOfScene.empty())
	{
		aiNode *node = childrenNodeOfScene.top();
		childrenNodeOfScene.pop();

		// Process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(processMesh(mesh, scene));
		}


		// Add children of the node into the stack so that they can be processed later
		if (node->mNumChildren > 0)
		{
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				childrenNodeOfScene.push(node->mChildren[i]);
			}
		}
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	// Mesh data to be filled
	std::vector<MeshVertex> vertices;
	std::vector<MeshTexture> textures;
	std::vector<unsigned int> indices;

	// Iterate over each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		glm::vec3 vector; // Declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		
		// Get position of vertiex
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.positions = vector;

		// Get normal of vertex
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normals = vector;
		}
		else
		{
			vertex.normals = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		// Get texture coordinates
		if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 texCoords;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;

			vertex.texCoords = texCoords;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// Iterate through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// Assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. Diffuse Maps
	vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. Specular Maps
	vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// Construct mesh object from extracted data
	return Mesh(vertices, textures, indices, this->shaderProgramID);
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
	std::vector<MeshTexture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading new texture
		bool skip = false;
		for (unsigned int j = 0; j < this->texturesLoaded.size(); j++)
		{
			if (std::strcmp(this->texturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(this->texturesLoaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{
			// If texture hasn't been loaded already, load it
			MeshTexture texture;
			texture.id = TextureFromFile(str.C_Str(), this->modelDirectory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture); // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	return textures;
}

static unsigned int TextureFromFile(const char *path, const string &directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	Texture *texture = new Texture(filename.c_str());
	return texture->textureID;
}
