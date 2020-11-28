#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Globals.h"
#include "Module.h"
#include "Event.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{

public:

	SDL_Window* window			= nullptr;	//The window we'll be rendering to
	SDL_Surface* screen_surface = nullptr;	//The surface contained by the window

private:
	int		width	= 0;
	int		height	= 0;
	int		fullwidth	= 0;
	int		fullheight	= 0;
	bool	fullscreen = FULLSCREEN;
	bool	borderless = BORDERLESS;

public:

	ModuleWindow();
	~ModuleWindow();

	// ----------- Module Functions ---------- //
	bool Init();
	bool CleanUp();
	void ReceiveEvent(const Event& event);

	// ---------- Getters & Setters ---------- //
	float GetAspectRatio() const { return (App->window->width) / (float)(App->window->height); }

	void SetWidth(int _width)	{ width = _width; }
	void SetHeight(int _height)	{ height = _height; }

	// ------------ Module Window ------------ //
	void ToggleFullscreen();
	void ToggleBorderless();
};

#endif // __ModuleWindow_H__