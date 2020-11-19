#pragma once
#include "Module.h"
#include "Globals.h"

class ResourceTexture
{

public:

	ResourceTexture();
	~ResourceTexture();

	static unsigned int LoadTexture(const char* file_name, const char* mesh_path);

};

