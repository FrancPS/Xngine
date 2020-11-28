
#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "WindowConfig.h"
#include "WindowConsole.h"
#include "WindowProperties.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"


ModuleEditor::ModuleEditor() {
    windows.push_back(winConfig = new WindowConfig());
    windows.push_back(winConsole = new WindowConsole());
    windows.push_back(winProperties = new WindowProperties());
}
ModuleEditor::~ModuleEditor() {}

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
	
	Draw();

    if (aboutWindowShow)
        ShowAboutWindow();

	ImGui::Render();
    if (menuQuitSelected)
        return UPDATE_STOP;
    else
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
    ShowMainMenu();
	for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); ++it) {
		if ((*it)->isDisplayed)
			(*it)->Draw();
	}
}

void ModuleEditor::ShowMainMenu() {
    // Menu Bar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Layout"))
        {
            ImGui::MenuItem("Properties",       NULL, &winProperties->isDisplayed);
            ImGui::MenuItem("Configuration",    NULL, &winConfig->isDisplayed);
            ImGui::MenuItem("Console",          NULL, &winConsole->isDisplayed);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("About"))
        {
            if (ImGui::MenuItem("Github Page...")) { ShellExecute(NULL, "open", "https://github.com/FrancPS/Xngine", NULL, NULL, SW_SHOWNORMAL); }
            if (ImGui::MenuItem("Download Latest...")) { ShellExecute(NULL, "open", "https://github.com/FrancPS/Xngine/releases", NULL, NULL, SW_SHOWNORMAL); }
            ImGui::Separator();
            if (ImGui::MenuItem("About", NULL, aboutWindowShow)) { aboutWindowShow = !aboutWindowShow; }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::ShowMenuFile()
{
    ImGui::MenuItem("(demo menu)", NULL, false, false);
    ImGui::MenuItem("Only QUIT option is actually working!", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}

    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Colors"))
    {
        float sz = ImGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
        }
        ImGui::EndMenu();
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Quit Application", "ESC")) { menuQuitSelected = true; }
}

void ModuleEditor::ShowAboutWindow() {
    ImGui::Begin("ABOUT");
    ImGui::TextWrapped("THIS IS THE ABOUT WINDOW.\nI don't know what to put in here so I will just write some random stuff, like my name:\n Francesc Porta Solsona\nAnd so on we could write here the specifications of the engine and other stuff!");
    ImGui::End();
}

void ModuleEditor::ForwardLog(const char* const _string) const {
    winConsole->AddLog(_string);
}