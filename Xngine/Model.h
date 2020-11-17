#pragma once
#include "Globals.h"
#include "Mesh.h"
#include "assimp/scene.h"
#include <vector>

class Model
{
public:

	Model();
	~Model();

	std::vector<Mesh> GetMeshes() const { return modelMeshes; }
	std::vector<unsigned int> GetTextures() const { return modelMaterials; }

	void Load(const char* file_name);
	void Draw();

private:
	void LoadTextures(aiMaterial** _mMaterials, unsigned int _mNumMaterials);
	void LoadMeshes(const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

private:
	std::vector<Mesh> modelMeshes;
	unsigned int numMeshes = 0;
	std::vector<unsigned int> modelMaterials;
};

