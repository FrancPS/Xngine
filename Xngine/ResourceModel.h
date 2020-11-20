#ifndef __ResourceModel_H__
#define __ResourceModel_H__
#include "Globals.h"
#include "ResourceMesh.h"
#include "assimp/scene.h"
#include <vector>

class ResourceModel
{

private:

	std::vector<ResourceMesh*>	modelMeshes;
	unsigned int				numMeshes = 0;
	std::vector<unsigned int>	modelMaterials;
	// we only use 1 material for now


public:

	ResourceModel();
	~ResourceModel();

	// ---------- Getters & Setters ---------- //
	std::vector<ResourceMesh*> GetMeshes() const { return modelMeshes; }
	std::vector<unsigned int> GetTextures() const { return modelMaterials; }

	void Load(const char* file_name);
	void UnLoad();
	void Draw();

private:

	void LoadMeshes(const aiScene* scene);
	void LoadMaterials(const aiScene* scene, const char* file_name);

};
#endif // __ResourceModel_H__
