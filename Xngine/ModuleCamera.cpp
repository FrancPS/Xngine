#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "Geometry/Frustum.h"
#include "Math/float3x3.h"
#include "SDL.h"
#include "GL/glew.h"

// Constructor
ModuleCamera::ModuleCamera()
{
	mTicksCount = 0;
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

#pragma region // ----------- Module Functions ---------- //
bool ModuleCamera::Init()
{
	LOG("Creating Camera context");

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	frustum.SetPos(float3(0, 1, -2));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update() 
{
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 100.f; // Get deltaTime of current frame

	/// ---------------------- KEYS ---------------------- ///
	// ---- TRANSLATION COMMANDS --- //
	// W -- Move Forward
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			MoveFrontAxis(deltaTime*2);
		else
			MoveFrontAxis(deltaTime);
	}
	// S -- Move Backward
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			MoveFrontAxis(-deltaTime * 2);
		else
			MoveFrontAxis(-deltaTime);
	}
	// D -- Move Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			MoveRightAxis(deltaTime * 2);
		else
			MoveRightAxis(deltaTime);
	}
	// A -- Move Left
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			MoveRightAxis(-deltaTime * 2);
		else
			MoveRightAxis(-deltaTime);
	}
	// Q -- Move Unit Up
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
		MoveYAxis(1);
	}
	// E -- Move Unit Down
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		MoveYAxis(-1);
	}

	// ----- ROTATION COMMANDS ----- //
	// KeyUP -- Pitch ++
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			Pitch(deltaTime);
		else
			Pitch(deltaTime/2);
	}
	// KeyDOWN -- Pitch --
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			Pitch(-deltaTime);
		else
			Pitch(-deltaTime/2);
	}
	// KeyLEFT -- Yaw ++
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			Yaw(deltaTime);
		else
			Yaw(deltaTime/2);
	}
	// KeyRIGHT -- Yaw --
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && !(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)) {
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			Yaw(-deltaTime);
		else
			Yaw(-deltaTime/2);
	}

	/// ---------------------- MOUSE --------------------- ///
	// WHEEL -- Move In/Out
	if (App->input->GetMouseWheel() != 0) {
		MoveFrontAxis(App->input->GetMouseWheel()); // Moves in units [-int ... +int]
	}

	// RIGHT CLICK -- Rotation
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		Yaw(App->input->GetMouseMotion().x * deltaTime);
		Pitch(App->input->GetMouseMotion().y * deltaTime);
	}

	
	mTicksCount = SDL_GetTicks();	// Update tick counts (deltaTime for next frame)
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	//Send the frustum view matrix to OpenGL
	projectionMatrix = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	LOG("Destroying camera");
	
	return true;
}
#pragma endregion 


#pragma region // ---------- Getters & Setters ---------- //
void ModuleCamera::SetFOV(unsigned width, unsigned height) {
	float aspectRatio = width / height;
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f * aspectRatio);
}
#pragma endregion 


#pragma region // ------------ Module Camera ------------ //
bool ModuleCamera::LookAt(int _x, int _y, int _z) {
	float3x3	rotationMatrix = float3x3::LookAt(frustum.Front(), float3::float3(_x, _y, _z), frustum.Up(), float3::float3(0, 1, 0));
	vec			oldFront		= frustum.Front().Normalized();
	vec			oldUp			= frustum.Up().Normalized();

	frustum.SetFront(rotationMatrix * oldFront);
	frustum.SetUp(rotationMatrix * oldUp);
	return true;
}
#pragma endregion


#pragma region // ---------- Camera Traslations --------- //
bool ModuleCamera::MoveFrontAxis(float _speed)
{
	float4x4 translationM = float4x4::identity;
	translationM.SetTranslatePart(frustum.Front()* _speed);
	frustum.SetPos(translationM.TransformPos(frustum.Pos()));

	return true;
}

bool ModuleCamera::MoveRightAxis(float _speed)
{
	float4x4 translationM = float4x4::identity;
	translationM.SetTranslatePart(frustum.WorldRight() * _speed);
	frustum.SetPos(translationM.TransformPos(frustum.Pos()));

	return true;
}

bool ModuleCamera::MoveYAxis(int upOrDown) {
	frustum.SetPos(frustum.Pos()+float3::float3(0, upOrDown, 0));

	return true;
}
#pragma endregion


#pragma region // ----------- Camera Rotations ---------- //
bool ModuleCamera::Pitch(float _speed) {
	float3x3	rotationMatrix = float3x3::RotateAxisAngle(frustum.WorldRight(), _speed);
	vec			newFront = rotationMatrix * frustum.Front().Normalized();
	vec			newUp = rotationMatrix * frustum.Up().Normalized();

	if (newFront.AngleBetweenNorm(float3::float3(0, 1, 0)) > 0.1f && newFront.AngleBetweenNorm(float3::float3(0, -1, 0)) > 0.1f) {	// Limit angle from vertical to 0.1rad //TODO bug: repeatedly rotating with the mouse bracks the condition!
		frustum.SetFront(newFront);
		frustum.SetUp(newUp);
	}
	return true;
}

bool ModuleCamera::Yaw(float _speed) {
	float3x3	rotationMatrix = float3x3::RotateY(_speed);
	vec			oldFront = frustum.Front().Normalized();
	vec			oldUp = frustum.Up().Normalized();

	frustum.SetFront(rotationMatrix * oldFront);
	frustum.SetUp(rotationMatrix * oldUp);
	return true;
}

#pragma endregion