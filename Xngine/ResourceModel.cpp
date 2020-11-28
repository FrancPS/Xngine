#include "Globals.h"
#include "Application.h"
#include "ResourceModel.h"
#include "ResourceMesh.h"
#include "ModuleTexture.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "GL/glew.h"

void AssimpCallback(const char* message, char* userData) {
	if (message) LOG("Assimp Message: %s", message);
}

ResourceModel::ResourceModel() {
	struct aiLogStream stream;
	stream.callback = AssimpCallback;
	aiAttachLogStream(&stream);
}

ResourceModel::~ResourceModel() {
	unsigned int i;
	// destroy objects from mesh list
	for (i = 0; i < modelMeshes.size(); i++)
	{
		delete modelMeshes[i];
		modelMeshes[i] = nullptr;
	}
	// erase modelMaterials
	for (i = 0; i < modelMaterials.size(); i++)
	{
		glDeleteTextures(1, &modelMaterials[i]);
	}
}


void ResourceModel::Load(const char* file_name)
{
	const aiScene* scene;
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		UnLoad();
		
		LoadMaterials(scene, file_name);
		LoadMeshes(scene);

		LOG("Scene loaded correctly");
	}
	else
	{
		// the file was not a model, but it can be a texture!
		UnLoadMaterials();
		unsigned int texID = App->textures->LoadTexture(file_name, file_name);
		if (texID)
			modelMaterials.push_back(texID);
		else
			LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void ResourceModel::LoadMaterials(const aiScene* scene, const char* file_name)
{
	aiString file;
	modelMaterials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			// Populate materials list
			modelMaterials.push_back(App->textures->LoadTexture(file.data, file_name));
		}
	}
	LOG("Materials loaded correctly");
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
	LOG("Meshes loaded correctly");
}

void ResourceModel::Draw() {
	for (unsigned int i = 0; i < numMeshes; ++i) {
		modelMeshes[i]->Draw(modelMaterials);
	}
}

void ResourceModel::UnLoad()
{
	LOG("Unloading the current module");
	// destroy objects from mesh list
	for (unsigned int i = 0; i < modelMeshes.size(); i++)
	{
		delete modelMeshes[i];
	}
	numMeshes = 0;
	modelMeshes.clear();

	// erase modelMaterials
	UnLoadMaterials();
}

void ResourceModel::UnLoadMaterials() {
	for (unsigned int i = 0; i < modelMaterials.size(); i++)
	{
		glDeleteTextures(1, &modelMaterials[i]);
	}
	modelMaterials.clear();
}
