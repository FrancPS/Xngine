#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Globals.h"
#include <string>


class ModuleTexture : public Module
{
public:
	// This is valid because we are loading only one texture!
	unsigned int texWidth	= 0;
	unsigned int texHeight	= 0;
	unsigned int textureID	= 0;

public:

	ModuleTexture();
	~ModuleTexture();

	bool Init();;

	const unsigned int LoadTexture(const char* file_name, const char* mesh_path);

private:
	std::string getPathName(const std::string& s);
};

#endif // __ModuleTexture_H__