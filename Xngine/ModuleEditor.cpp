
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"


ModuleEditor::ModuleEditor() {
}
ModuleEditor::~ModuleEditor() {
}

#pragma region // ----------- Module Functions ---------- //
bool ModuleEditor::Init() {
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());
	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleEditor::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update() {
	
	ImGui::ShowDemoWindow();
	Draw();
	ImGui::Render();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {
	
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp() {

	for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); ++it)
		(*it)->CleanUp();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown(); 
	ImGui::DestroyContext();
	return true;
}
#pragma endregion

void ModuleEditor::Draw() {
	for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); ++it)
		(*it)->Draw();
}