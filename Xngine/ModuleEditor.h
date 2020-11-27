#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Globals.h"
#include "Module.h"
#include "Window.h"
#include <list>

class WindowConfig;
class WindowConsole;
class WindowProperties;


class ModuleEditor : public Module
{

private:
	WindowConfig*		winConfig		= nullptr;
	WindowConsole*		winConsole		= nullptr;
	WindowProperties*	winProperties	= nullptr;

	std::list<Window*> windows;

	bool menuQuitSelected = false;
	bool aboutWindowShow = false;

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
	bool ShowMainMenu();
	void ShowMenuFile();
	void ShowAboutWindow();


	friend class WindowMenu;		// is this better than a getter for each win* ?
};
#endif // __ModuleEditor_H__
