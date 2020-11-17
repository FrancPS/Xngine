#pragma once

#include "Globals.h"
#include "assimp/mesh.h"
#include <vector>


struct Vertex {
	int x;
	int y;
	int z; //use vec3?
	int uvX;
	int uvY; // use vec2
};

class ResourceMesh
{
public:
	//std::vector<Vertex> vertices;
	unsigned int numVertex;

	unsigned int materialIndex = 0;
	unsigned int numIndices = 0;

	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;

public:
	ResourceMesh(const aiMesh* mesh);

	void Draw(const std::vector<unsigned int>& model_textures);

private:
	void LoadMeshVBO(const aiMesh* mesh);
	void LoadMeshEBO(const aiMesh* mesh);
	void CreateVAO();

};

