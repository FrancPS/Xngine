#include "Application.h"
#include "ResourceMesh.h"
#include "Globals.h"
#include "ResourceProgram.h"
#include "ModuleCamera.h"
#include "GL/glew.h"
#include "MathGeoLib-master/Math/float4x4.h"




ResourceMesh::ResourceMesh(const aiMesh* _mesh){
	LoadMeshVBO(_mesh);
	LoadMeshEBO(_mesh);
	CreateVAO();
}


void ResourceMesh::LoadMeshVBO(const aiMesh* mesh) {
	materialIndex = mesh->mMaterialIndex;
	numVertex = mesh->mNumVertices;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active

	unsigned int vSize = (sizeof(float)) * 5 * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, vSize, nullptr, GL_STATIC_DRAW);

	float* vertexData = (float*)(glMapBufferRange(GL_ARRAY_BUFFER, 0, vSize, GL_MAP_WRITE_BIT));

	unsigned int pos = 0;

	// copied as interleaved array. Good for static meshes!
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		/*// Position
		vertices[i].x = mesh->mVertices[i].x;
		vertices[i].z = mesh->mVertices[i].y;
		vertices[i].x = mesh->mVertices[i].z;
		// Normals
		// ...
		// UV
		vertices[i].uvX = mesh->mTextureCoords[0][i].x;
		vertices[i].uvY = mesh->mTextureCoords[0][i].y;*/

		// load info into vbo
		vertexData[pos++] = mesh->mVertices[i].x;
		vertexData[pos++] = mesh->mVertices[i].y;
		vertexData[pos++] = mesh->mVertices[i].z;
		vertexData[pos++] = mesh->mTextureCoords[0][i].x;
		vertexData[pos++] = mesh->mTextureCoords[0][i].y;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void ResourceMesh::LoadMeshEBO(const aiMesh* mesh) {
	numIndices = mesh->mNumFaces * 3;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);

	unsigned int* indices = (unsigned int*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void ResourceMesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)0); // vertices
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 3)); // uv
	glBindVertexArray(0);
}

void ResourceMesh::Draw(const std::vector<unsigned int>& model_textures)
{
	// -- Create Program -- // THIS SHOULD NOT EXECUTE EVERY FRAME!!
	unsigned vertexTriangle = ResourceProgram::CompileShader(GL_VERTEX_SHADER, ResourceProgram::LoadShaderSource("../Shaders/default_VertexShader.glsl"));
	unsigned fragmentTriangle = ResourceProgram::CompileShader(GL_FRAGMENT_SHADER, ResourceProgram::LoadShaderSource("../Shaders/default_FragmentShader.glsl"));
	unsigned program = ResourceProgram::CreateProgram(vertexTriangle, fragmentTriangle); 

	const float4x4& view = App->camera->GetViewMatrix();
	const float4x4& proj = App->camera->GetProjectionMatrix();
	float4x4 model = float4x4::identity;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[materialIndex]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}