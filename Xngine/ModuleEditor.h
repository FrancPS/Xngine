#pragma once

#include "Globals.h"
#include "Module.h"


class ModuleEditor : public Module
{


public:

	ModuleEditor();
	~ModuleEditor();

	// ----------- Module Functions ---------- //
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	void DrawMenu();
};

