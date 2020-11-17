#pragma once
#include "Globals.h"
#include "ResourceMesh.h"
#include "assimp/scene.h"
#include <vector>

class ResourceModel
{
public:

	ResourceModel();
	~ResourceModel();

	std::vector<ResourceMesh> GetMeshes() const { return modelMeshes; }
	std::vector<unsigned int> GetTextures() const { return modelMaterials; }

	void Load(const char* file_name);
	void Draw();

private:
	void LoadTextures(aiMaterial** _mMaterials, unsigned int _mNumMaterials);
	void LoadMeshes(const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

private:
	std::vector<ResourceMesh> modelMeshes;
	unsigned int numMeshes = 0;
	std::vector<unsigned int> modelMaterials;
};

