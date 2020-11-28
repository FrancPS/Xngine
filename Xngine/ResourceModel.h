#ifndef __ResourceModel_H__
#define __ResourceModel_H__
#include "Globals.h"
#include "ResourceMesh.h"
#include "assimp/scene.h"
#include <vector>

class ResourceModel
{

private:

	unsigned int				numMeshes = 0;
	std::vector<ResourceMesh*>	modelMeshes;
	// we only use 1 material for now
	std::vector<unsigned int>	modelMaterials;


public:

	ResourceModel();
	~ResourceModel();

	// ---------- Getters & Setters ---------- //
	std::vector<ResourceMesh*> GetMeshes() const { return modelMeshes; }
	std::vector<unsigned int> GetTextures() const { return modelMaterials; }
	unsigned int GetNumMeshes() const { return numMeshes; }

	void Load(const char* const file_name);
	void Draw();

private:

	void LoadMeshes(const aiScene* const scene);
	void LoadMaterials(const aiScene* const scene, const char* const file_name);
	void UnLoad();
	void UnLoadMaterials();


	friend class WindowProperties;
};
#endif // __ResourceModel_H__
