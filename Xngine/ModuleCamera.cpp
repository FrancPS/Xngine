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

// Called before render is available
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
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 100.f;
	/// ---------------------- KEYS ---------------------- ///
	// TRASLATION COMMANDS
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

	// ROTATION COMMANDS
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
		Yaw(App->input->GetMouseMotion().x*deltaTime/10);
		Pitch(App->input->GetMouseMotion().y * deltaTime/10);
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	//Send the frustum view matrix to OpenGL
	projectionGL = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{
	LOG("Destroying camera");

	return true;
}

/* TRASLATION */
#pragma region TRASLATIONS
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

/* ROTATION */
#pragma region ROTATIONS
bool ModuleCamera::Pitch(float _speed) {
	float3x3 rotationMatrix = float3x3::RotateAxisAngle(frustum.WorldRight(), _speed);
	vec newFront = rotationMatrix * frustum.Front().Normalized();
	vec newUp = rotationMatrix * frustum.Up().Normalized();

	if (newFront.AngleBetweenNorm(float3::float3(0, 1, 0)) > 0.1f && newFront.AngleBetweenNorm(float3::float3(0, -1, 0)) > 0.1f) {		// Limit angle from vertical to 0.1rad    /////////////////////////////////////////////WORLD UP???
		frustum.SetFront(newFront);
		frustum.SetUp(newUp);
	}
	return true;
}

bool ModuleCamera::Yaw(float _speed) {
	float3x3 rotationMatrix = float3x3::RotateY(_speed); // Yaws in the absolute Y axis, its less disorienting
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix * oldFront);
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix * oldUp);
	return true;
}

#pragma endregion


bool ModuleCamera::LookAt(int _x, int _y, int _z) {
	float3x3 rotationMatrix = float3x3::LookAt(frustum.Front(), float3::float3(_x,_y,_z), frustum.Up(), float3::float3(0, 1, 0));
	vec oldFront = frustum.Front().Normalized();
	frustum.SetFront(rotationMatrix * oldFront);
	vec oldUp = frustum.Up().Normalized();
	frustum.SetUp(rotationMatrix * oldUp);
	return true;
}

bool ModuleCamera::SetPlaneDistances(float _zNear, float _zFar) {
	frustum.SetViewPlaneDistances(_zNear, _zFar);
	return true;
}

bool ModuleCamera::SetFOV(unsigned width, unsigned height) {
	float aspectRatio = width / height;
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f*aspectRatio);
	return true;
}
