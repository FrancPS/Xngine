#include "Globals.h"
#include "Application.h"
#include "Model.h"
#include "ModuleTexture.h"
#include "Mesh.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"


Model::Model() {

}
Model::~Model() {
	// remove elements from the lists //
	//remove meshes created //
}


void Model::Load(const char* file_name)
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

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	modelMaterials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			modelMaterials.push_back(App->textures->LoadTexture(file.data));
		}
	}
}

void Model::LoadMeshes(const aiScene* scene) {
	aiString file;
	modelMeshes.reserve(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		// Populate meshes list
		modelMeshes.push_back(Mesh(scene->mMeshes[i]));
		numMeshes++;
	}
}

void Model::LoadTextures(aiMaterial** _mMaterials, unsigned int _mNumMaterials) 
{

}

void Model::Draw() {
	for (unsigned int i = 0; i < numMeshes; ++i) {
		modelMeshes[i].Draw(modelMaterials);
	}
}
