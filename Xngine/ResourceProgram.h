#pragma once

#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"

class ResourceProgram
{
public:
	// Module Functions
	ResourceProgram();
	~ResourceProgram();

	static unsigned CompileShader(unsigned type, const char* source);
	static char* LoadShaderSource(const char* shader_file_name);
	static unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
};

