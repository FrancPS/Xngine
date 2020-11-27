#ifndef __ResourceMesh_H__
#define __ResourceMesh_H__

#include "Globals.h"
#include "assimp/mesh.h"
#include <vector>

class ResourceMesh
{
public:

	unsigned int numVertex = 0;

	unsigned int materialIndex	= 0;
	unsigned int numIndices		= 0;

	unsigned int vbo = 0;
	unsigned int ebo = 0;
	unsigned int vao = 0;

private:

	unsigned int shaderProgram = 0;


public:

	ResourceMesh(const aiMesh* mesh);
	~ResourceMesh();
	void Draw(const std::vector<unsigned int>& model_textures);

private:

	void LoadMeshVBO(const aiMesh* mesh);
	void LoadMeshEBO(const aiMesh* mesh);
	void CreateVAO();

};
#endif // __ResourceMesh_H__
