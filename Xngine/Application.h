#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleCamera;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleProgram;
class ModuleTexture;
class ModuleModel;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleDebugDraw* debug_draw = nullptr;
	ModuleProgram* program = nullptr;
	ModuleTexture* textures = nullptr;
	ModuleModel* model = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
