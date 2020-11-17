#pragma once
#include "Module.h"
#include "Model.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void* GetContext() const { return this->context; }
	void SetContext(void* _context) { this->context = _context; }

	Model bakerhouse;

private:
	void* context;
	//unsigned int VBO;
	unsigned vboTriangle;
	unsigned programTriangle;


	


private:
	void RenderTriangle();
	
	
};
