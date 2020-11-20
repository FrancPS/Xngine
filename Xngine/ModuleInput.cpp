#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Event.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "SDL.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput()
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	wheel = 0;
}

ModuleInput::~ModuleInput()
{}

#pragma region // ----------- Module Functions ---------- //
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	mouse_motion = { 0, 0 };	// reset the value of the mouse motions for this frame
	wheel = 0.0f;
	memset(windowEvents, false, WE_COUNT * sizeof(bool));
	ImGuiIO& io = ImGui::GetIO();
	bool imguiHasInputs = io.WantCaptureMouse || io.WantCaptureKeyboard;

	// ----- KEYBOARD ----- //
	if (!imguiHasInputs)
	{
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < MAX_KEYS; ++i)
		{
			if (keys[i] == 1)
			{
				if (keyboard[i] == KEY_IDLE)
					keyboard[i] = KEY_DOWN;
				else
					keyboard[i] = KEY_REPEAT;
			}
			else
			{
				if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
					keyboard[i] = KEY_UP;
				else
					keyboard[i] = KEY_IDLE;
			}
		}

		for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
		{
			if (mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_REPEAT;

			if (mouse_buttons[i] == KEY_UP)
				mouse_buttons[i] = KEY_IDLE;
		}
	}


	// ----- MOUSE AND WINDOW ----- //
	while (SDL_PollEvent(&event) != 0)
	{
		// Hardcoded Imgui events
		if (imguiHasInputs) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			return UPDATE_CONTINUE;
		}

		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				Event ev(Event::window_resize);
				ev.point2d.x = event.window.data1;
				ev.point2d.y = event.window.data2;
				App->BroadcastEvent(ev);
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel;
			mouse_motion.y = event.motion.yrel;
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			break;

		case SDL_MOUSEWHEEL:
			wheel = (float)event.wheel.y;
			break;
		case SDL_DROPFILE:      // In case if dropped file
			Event ev(Event::file_dropped);
			ev.string.ptr = event.drop.file;
			App->BroadcastEvent(ev);
			SDL_free(event.drop.file);
			break;
		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

update_status ModuleInput::Update()
{
    return UPDATE_CONTINUE;
}

update_status ModuleInput::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	delete[] keyboard;
	keyboard = nullptr;
	return true;
}
#pragma endregion