#pragma once
#include "Module.h"
#include "ResourceModel.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{

private:
	void* context;
	ResourceModel bakerhouse;

public:

	ModuleRender();
	~ModuleRender();

	// ----------- Module Functions ---------- //
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void ReceiveEvent(const Event& event);

	// ---------- Getters & Setters ---------- //
	void* GetContext() const		{ return this->context; }
	void SetContext(void* _context) { this->context = _context; }
};
