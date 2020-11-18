
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
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

	//DRAWLOG("CONSOLE");	// Print logs to App console // !!! IT is creating the window every frame, even if no logs are sent (putting DRAWLOG inside LOG : errors when LOGs are called in main.cpp)
	DrawMenu();

	ImGui::Render();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate() {
	
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown(); 
	ImGui::DestroyContext();
	return true;
}
#pragma endregion



void ModuleEditor::DrawMenu() {
	ImGui::Begin("MENU");
	
	ImGui::End();
	/*ImGui::Begin("MENU");
	
	if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples"))
        {
            ImGui::MenuItem("Main menu bar", NULL);
            ImGui::MenuItem("Console", NULL);
            ImGui::MenuItem("Log", NULL);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
			ImGui::MenuItem("Main menu bar", NULL);
			ImGui::MenuItem("Console", NULL);
			ImGui::MenuItem("Log", NULL);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
	}

	ImGui::End();*/
}  