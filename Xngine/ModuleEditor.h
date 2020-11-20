#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Globals.h"
#include "Module.h"
#include "Window.h"
#include <list>


class ModuleEditor : public Module
{

private:

	std::list<Window*> windows;

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

	void Draw();
};
#endif // __ModuleEditor_H__
