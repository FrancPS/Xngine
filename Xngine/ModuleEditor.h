#pragma once

#include "Globals.h"
#include "Module.h"
#include <vector>


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

	std::vector<const char*> Buf;
	bool ScrollToBottom;

private:
	void DrawMenu();
	void Draw(const char* title, bool* p_opened);
};

