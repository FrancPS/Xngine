#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	// Destructor
	virtual ~ModuleWindow();
	bool Init();
	bool CleanUp();		// Called before quitting

	const int GetWidth() const {
		return width;
	}
	const int GetHeight() const {
		return height;
	}

	void SetAspectRatio(int _width, int _height);

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	int width;
	int height;
};

#endif // __ModuleWindow_H__