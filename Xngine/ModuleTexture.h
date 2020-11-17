#pragma once
#include "Module.h"
#include "Globals.h"


class ModuleTexture : public Module
{

public:

	ModuleTexture();
	~ModuleTexture();

	bool Init();;

	//GLuint GetTexture() { return tex;  }
	unsigned int LoadTexture(const char* file_name);
private:

public:

};

