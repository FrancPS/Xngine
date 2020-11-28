#ifndef __ResourceProgram_H__
#define __ResourceProgram_H__

#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"

class ResourceProgram //this could be a struct?
{
public:
	ResourceProgram();
	~ResourceProgram();

	static char*		LoadShaderSource(const char* const shader_file_name);
	static unsigned int	CompileShader(const unsigned int type, const char* const source);
	static unsigned	int CreateProgram(const unsigned int vtx_shader, const unsigned int frg_shader);
};
#endif // __ResourceProgram_H__
