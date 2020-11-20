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
class ModuleDebugDraw;

struct Event;

class Application
{

public:

	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleDebugDraw* debug_draw = nullptr;

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