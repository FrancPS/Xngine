#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "Event.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{

public:

	SDL_Window* window = NULL;			//The window we'll be rendering to
	SDL_Surface* screen_surface = NULL;	//The surface contained by the window

private:
	int		width;
	int		height;
	bool	fullscreen;
	int		fullwidth;
	int		fullheight;
	bool	borderless;

public:

	ModuleWindow();
	~ModuleWindow();

	// ----------- Module Functions ---------- //
	bool Init();
	bool CleanUp();
	void ReceiveEvent(const Event& event);

	// ---------- Getters & Setters ---------- //
	float GetAspectRatio() { return (App->window->width) / (float)(App->window->height); }

	void SetWidth(int _width)	{ width = _width; }
	void SetHeight(int _height)	{ height = _height; }

	// ------------ Module Window ------------ //
	void ToggleFullscreen();
	void ToggleBorderless();
};

#endif // __ModuleWindow_H__