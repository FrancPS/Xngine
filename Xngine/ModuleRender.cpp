#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Geometry/Frustum.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}

ModuleRender::ModuleRender() {}

// Destructor
ModuleRender::~ModuleRender() {}

#pragma region 	// ----------- Module Functions ---------- //
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	// -- openGL INIT -- //
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GLContext glcontext = SDL_GL_CreateContext(App->window->window);
	this->context = glcontext;

	// -- GLEW INIT -- //
	GLenum err = glewInit();

	// Send GPU, OpenGL & glew info to App
	App->vOpenGL	= (char*)glGetString(GL_VERSION);
	App->vGlew		= (char*)glewGetString(GLEW_VERSION);
	App->gpu		= (char*)glGetString(GL_RENDERER);

# ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
# endif

	bakerhouse->Load("BakerHouse.fbx");

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update()
{
	// Draw the grid and origin
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	App->debug_draw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), w, h, gridColor.xyz());

	// Draw the model
	bakerhouse->Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(this->context);
	delete bakerhouse;
	bakerhouse = nullptr;
	return true;
}

void ModuleRender::ReceiveEvent(const Event& event)
{
	switch (event.type)
	{
	case Event::window_resize:
	case Event::window_fullscreen:
		glViewport(0, 0, event.point2d.x, event.point2d.y);
		App->camera->SetFOV(event.point2d.x, event.point2d.y);
		break;
	case Event::file_dropped:
		LOG("A file has been dropped %s", event.string.ptr);
		bakerhouse->Load(event.string.ptr);
	}
}
#pragma endregion


