#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"

typedef unsigned __int8 Uint8;

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	iPoint		mouse_motion;
	iPoint		mouse;
	float		wheel;


public:
	
	ModuleInput();
	~ModuleInput();

	// ----------- Module Functions ---------- //
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	// ---------- Getters & Setters ---------- //
	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const				{ return keyboard[id]; }
	KeyState GetMouseButtonDown(int id) const	{ return mouse_buttons[id - 1]; }

	// Check for window events last frame
	bool GetWindowEvent(EventWindow ev) const	{ return windowEvents[ev]; }

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const		{ return mouse_motion; }
	const iPoint& GetMousePosition() const		{ return mouse; }
	const float& GetMouseWheel() const			{ return wheel; }

};
#endif // __ModuleInput_H__