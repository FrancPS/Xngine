#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow()
{
	fullscreen = FULLSCREEN;
	borderless = BORDERLESS;
}

ModuleWindow::~ModuleWindow()
{
}

#pragma region 	// ----------- Module Functions ---------- //
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Get screen size
		SDL_DisplayMode dm;

		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			return 1;
		}
		fullwidth	= dm.w;
		fullheight	= dm.h;
		width		= fullwidth /1.3;
		height		= fullheight /1.3;

		// Create window
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}
		if (BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			screen_surface = SDL_GetWindowSurface(window);	// Get window surface
		}
	}

	// Send CPU & RAM Hardware info to App
	App->cpuCores		= SDL_GetCPUCount();
	App->cpuCacheSize	= SDL_GetCPUCacheLineSize();
	App->systemRAM		= SDL_GetSystemRAM();
	App->hasAVX			= SDL_HasAVX();
	App->hasAVX2		= SDL_HasAVX2();
	App->hasAltiVec		= SDL_HasAltiVec();
	App->hasMMX			= SDL_HasMMX();
	App->hasRDTSC		= SDL_HasRDTSC();
	App->hasSSE			= SDL_HasSSE();
	App->hasSSE2		= SDL_HasSSE2();
	App->hasSSE3		= SDL_HasSSE3();
	App->hasSSE41		= SDL_HasSSE41();
	App->hasSSE42		= SDL_HasSSE42();
	// Send SDL info to App
	SDL_version linked;
	SDL_GetVersion(&linked);
	sprintf_s( App->vSDL, 10, "%d.%d.%d", linked.major, linked.minor, linked.patch);

	return ret;
}

bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::ReceiveEvent(const Event& event)
{
	switch (event.type)
	{
	case Event::window_resize:
		width = event.point2d.x;
		height = event.point2d.y;
		break;
	}
}
#pragma endregion

#pragma region// ------------ Module Window ------------ //
void ModuleWindow::ToggleFullscreen() {
	Event ev(Event::window_fullscreen);

	if (fullscreen) {
		SDL_SetWindowFullscreen(window, 0);
		ev.point2d.x = width;
		ev.point2d.y = height;
	}
	else {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		ev.point2d.x = fullwidth;
		ev.point2d.y = fullheight;
	}
	LOG("W%d,H%d", width, height);
	fullscreen = !fullscreen;
	App->BroadcastEvent(ev);
}

void ModuleWindow::ToggleBorderless() {
	LOG("%d", borderless);
	if (borderless)
		SDL_SetWindowBordered(window, SDL_TRUE);
	else
		SDL_SetWindowBordered(window, SDL_FALSE);
	borderless = !borderless;
}
#pragma endregion