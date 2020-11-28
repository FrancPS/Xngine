#ifndef __ResourceModel_H__
#define __ResourceModel_H__
#include "Globals.h"
#include "ResourceMesh.h"
#include "assimp/scene.h"
#include <vector>

class ResourceModel
{
public:
	float maxX = 0, maxY = 0, maxZ = 0;
	float minX = 3.40282e+038, minY = 3.40282e+038, minZ = 3.40282e+038; // MAXFLOAT*
	float sizeX = 0, sizeY = 0, sizeZ = 0;

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
	void ResetSizes();


	friend class WindowProperties;
};
#endif // __ResourceModel_H__
