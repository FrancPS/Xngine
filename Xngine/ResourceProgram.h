#ifndef __ResourceProgram_H__
#define __ResourceProgram_H__

#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"

class ResourceProgram
{
public:
	ResourceProgram();
	~ResourceProgram();

	static char*		LoadShaderSource(const char* shader_file_name);
	static unsigned int	CompileShader(unsigned type, const char* source);
	static unsigned	int CreateProgram(unsigned vtx_shader, unsigned frg_shader);
};
#endif // __ResourceProgram_H__
