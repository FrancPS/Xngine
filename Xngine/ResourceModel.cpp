#include "Globals.h"
#include "Application.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"


ResourceModel::ResourceModel() {

}

ResourceModel::~ResourceModel() {
	unsigned int i;
	// destroy objects from mesh list
	for (i = 0; i < modelMeshes.size(); i++)
	{
		delete modelMeshes[i];
	}
}


void ResourceModel::Load(const char* file_name)
{
	const aiScene* scene;
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LOG("Scene loaded corretly");
		
		LoadMaterials(scene);
		LoadMeshes(scene);
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void ResourceModel::LoadMaterials(const aiScene* scene)
{
	aiString file;
	modelMaterials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			// Populate materials list
			modelMaterials.push_back(ResourceTexture::LoadTexture(file.data));
		}
	}
}

void ResourceModel::LoadMeshes(const aiScene* scene) {
	aiString file;
	modelMeshes.reserve(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		// Populate meshes list
		modelMeshes.push_back(new ResourceMesh(scene->mMeshes[i]));
		numMeshes++;
	}
}

void ResourceModel::Draw() {
	for (unsigned int i = 0; i < numMeshes; ++i) {
		modelMeshes[i]->Draw(modelMaterials);
	}
}
