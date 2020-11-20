#ifndef __ResourceTexture_H__
#define __ResourceTexture_H__
#include "Module.h"
#include "Globals.h"

class ResourceTexture
{

public:

	ResourceTexture();
	~ResourceTexture();

	static unsigned int LoadTexture(const char* file_name, const char* mesh_path);

};
#endif // __ResourceTexture_H__
