#ifndef __Application_H__
#define __Application_H__

#include <vector>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleCamera;
class ModuleEditor;
class ModuleTexture;
class ModuleDebugDraw;

struct Event;

class Application
{

public:

	ModuleRender*		renderer	= nullptr;
	ModuleWindow*		window		= nullptr;
	ModuleInput*		input		= nullptr;
	ModuleCamera*		camera		= nullptr;
	ModuleEditor*		editor		= nullptr;
	ModuleTexture*		textures	= nullptr;
	ModuleDebugDraw*	debug_draw	= nullptr;

	// Hardware information //
	// in ModuleWindow.h
	int		systemRAM		= 0;
	int		cpuCores		= 0;
	int		cpuCacheSize	= 0;
	bool	hasAVX		= false;
	bool	hasAVX2		= false;
	bool	hasAltiVec	= false;
	bool	hasMMX		= false;
	bool 	hasRDTSC	= false;
	bool 	hasSSE		= false;
	bool 	hasSSE2		= false;
	bool 	hasSSE3		= false;
	bool 	hasSSE41	= false;
	bool 	hasSSE42	= false;
	const char* gpu		= nullptr;

	// Software
	char vSDL[10]{};
	const char* vOpenGL	= nullptr;
	const char* vGlew	= nullptr;

private:

	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void BroadcastEvent(const Event& event);
};

extern Application* App;

#endif // __Application_H__