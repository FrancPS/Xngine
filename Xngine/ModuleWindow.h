#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__ //TODO

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{

public:

	SDL_Window* window = NULL;			//The window we'll be rendering to
	SDL_Surface* screen_surface = NULL;	//The surface contained by the window

public:

	ModuleWindow();
	~ModuleWindow();

	// ----------- Module Functions ---------- //
	bool Init();
	bool CleanUp();
};

#endif // __ModuleWindow_H__