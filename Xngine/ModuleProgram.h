#pragma once

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib-master/Geometry/Frustum.h"

class ModuleProgram : public Module
{
public:
	// Module Functions
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	unsigned CompileShader(unsigned type, const char* source);
	char* LoadShaderSource(const char* shader_file_name);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

	unsigned programTriangle;

};

