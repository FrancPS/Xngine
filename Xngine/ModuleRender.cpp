#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "Model.h"
#include "debug-draw/ModuleDebugDraw.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib-master/Geometry/Frustum.h"
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

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
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
	// … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));// Should be 2.0
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

# ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
# endif

	// -- Create Triangle Program -- //
	unsigned vertexTriangle = App->program->CompileShader(GL_VERTEX_SHADER, App->program->LoadShaderSource("../Shaders/default_VertexShader.glsl"));
	unsigned fragmentTriangle = App->program->CompileShader(GL_FRAGMENT_SHADER, App->program->LoadShaderSource("../Shaders/default_FragmentShader.glsl"));

	programTriangle = App->program->CreateProgram(vertexTriangle, fragmentTriangle);

	vboTriangle = CreateTriangleVBO();

	bakerhouse.Load("BakerHouse.fbx");

	return true;
}

// Module Start(){}


update_status ModuleRender::PreUpdate()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	App->debug_draw->Draw(App->camera->GetViewMatrix(), App->camera->GetProjectionMatrix(), w, h);
	//RenderTriangle();
	bakerhouse.Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	// Destroy VBOs
	DestroyVBO(vboTriangle);
	LOG("Destroying renderer");
	SDL_GL_DeleteContext(this->context);
	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	App->camera->SetFOV(width, height);
}

// This function must be called one time at creation of vertex buffer
unsigned ModuleRender::CreateTriangleVBO()
{
	//float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float buffer_data[] = { 
		-1.0f, -1.0f, 0.0f, // v0 pos
		1.0f, -1.0f, 0.0f, // v1 pos
		1.0f, 1.0f, 0.0f, // v2 pos

		-1.0f, -1.0f, 0.0f, // v0 pos
		1.0f, 1.0f, 0.0f, // v2 pos
		-1.0f, 1.0f, 0.0f, // v3 pos

		0.0f, 0.0f, // v0 texcoord
		1.0f, 0.0f, // v1 texcoord
		1.0f, 1.0f, // v2 texcoord

		0.0f, 0.0f, // v0 texcoord
		1.0f, 1.0f, // v2 texcoord
		0.0f, 1.0f
	};
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	return vbo;
}
// This function must be called one time at destruction of vertex buffer
void ModuleRender::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

void ModuleRender::RenderTriangle()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	float4x4 projection = App->camera->GetProjectionMatrix();
	float4x4 model = float4x4::FromTRS(float3(0.0f, 1.0f, 2.0f), float4x4::RotateZ(pi), float3(1.0f, 1.0f, 0.0f));
	float4x4 view = App->camera->GetViewMatrix();

	glUseProgram(programTriangle);

	glUniformMatrix4fv(glGetUniformLocation(programTriangle, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programTriangle, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programTriangle, "proj"), 1, GL_TRUE, &projection[0][0]);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 6 * 3));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->textures->LoadTexture("Lenna.png")); /* Binding of texture name */ /*im loading the texture each frame! */
	glUniform1i(glGetUniformLocation(programTriangle, "mytexture"), 0);

	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

