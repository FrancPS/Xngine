#pragma once
#include "Module.h"
#include "Globals.h"


class ResourceTexture
{

public:

	ResourceTexture();
	~ResourceTexture();

	//GLuint GetTexture() { return tex;  }
	static unsigned int LoadTexture(const char* file_name);
private:

public:

};

