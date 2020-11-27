#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Globals.h"
#include "Module.h"
#include "ResourceModel.h"
#include "Event.h"
#include "Math/float4.h"
#include "Math/float3.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{

public:
	ResourceModel* bakerhouse = new ResourceModel(); //Must be public for WindowProperties. This is hardcoded, so nevermind
	float4 backgroundColor	= float4(0.1f, 0.1f, 0.1f, 1.0f);
	float4 gridColor		= float4(1.f, 1.f, 1.f, 1.f);

private:
	void* context = nullptr;

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
#endif // __ModuleRender_H__