#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.h"

class Model
{
public:
	Model(std::string const *path, GLuint shaderProgramID);
	~Model();
	void Draw(Shader *shader);
private:
	// Model Data
	std::vector<Mesh> meshes;
	std::vector<MeshTexture> texturesLoaded;
	std::string modelDirectory;
	GLuint shaderProgramID;
	
	void loadModel(std::string const *path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
};

#endif //MODEL_H 