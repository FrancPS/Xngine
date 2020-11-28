#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "Geometry/Frustum.h"
#include "Math/float3x3.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleCamera::ModuleCamera() {}

ModuleCamera::~ModuleCamera() {}

#pragma region // ----------- Module Functions ---------- //
bool ModuleCamera::Init()
{
	LOG("Creating Camera context");

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio((float)DEGTORAD * 90.0f, App->window->GetAspectRatio());

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
	mTicks = (float)(SDL_GetTicks() - mTicksCount);
	deltaTime = mTicks / 60.f; // Get deltaTime of current frame

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
		MoveYAxisUnit(1);
	}
	// E -- Move Unit Down
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		MoveYAxisUnit(-1);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		LookAt(0, 0, 0);
		// TODO: FUNCTION TO SELECT SPECIFIC OBJECT/coordinates
		// TODO: Smooth transition
		// TODO: Should place the camera near the object
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
		Yaw(-App->input->GetMouseMotion().x * deltaTime);
		Pitch(-App->input->GetMouseMotion().y * deltaTime);
	}

	// ALT + LClick -- Orbit
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)) {
		Orbit(-App->input->GetMouseMotion().x * deltaTime, App->input->GetMouseMotion().y * deltaTime, float3::float3(0,0,0));
	}

	mTicksCount = SDL_GetTicks();	// Update tick counts (deltaTime for next frame)

	//LOG("LOOKING AT: %f,%f,%f", frustum.Front().x, frustum.Front().y, frustum.Front().z);

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
void ModuleCamera::SetFOV(unsigned int width, unsigned int height) {
	float aspectRatio = width / (float)height;
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectRatio);
}
#pragma endregion 


#pragma region // ------------ Module Camera ------------ //
void ModuleCamera::LookAt(int _x, int _y, int _z) {
	float3 targetDir = float3((float)_x, (float)_y, (float)_z) - frustum.Pos();
	float3x3	rotationMatrix	= float3x3::LookAt(frustum.Front(), targetDir.Normalized(), frustum.Up(), float3::float3(0, 1, 0));
	vec			oldFront		= frustum.Front();
	vec			oldUp			= frustum.Up();
	frustum.SetFront((rotationMatrix * oldFront).Normalized());
	frustum.SetUp((rotationMatrix * oldUp).Normalized());
}
#pragma endregion


#pragma region // ---------- Camera Traslations --------- //
void ModuleCamera::MoveFrontAxis(float _speed)
{
	float4x4 translationM = float4x4::identity;
	translationM.SetTranslatePart(frustum.Front()* _speed);
	frustum.SetPos(translationM.TransformPos(frustum.Pos()));
}

void ModuleCamera::MoveRightAxis(float _speed)
{
	float4x4 translationM = float4x4::identity;
	translationM.SetTranslatePart(frustum.WorldRight() * _speed);
	frustum.SetPos(translationM.TransformPos(frustum.Pos()));
}

void ModuleCamera::MoveUpAxis(float _speed)
{
	float4x4 translationM = float4x4::identity;
	translationM.SetTranslatePart(frustum.Up() * _speed);
	frustum.SetPos(translationM.TransformPos(frustum.Pos()));
}

void ModuleCamera::MoveYAxisUnit(int upOrDown) {
	frustum.SetPos(frustum.Pos()+float3::float3(0, (float)upOrDown, 0));
}
#pragma endregion


#pragma region // ----------- Camera Rotations ---------- //
void ModuleCamera::Pitch(float _speed) {
	float3x3	rotationMatrix = float3x3::RotateAxisAngle(frustum.WorldRight(), _speed);
	vec			newFront = rotationMatrix * frustum.Front().Normalized();
	vec			newUp = rotationMatrix * frustum.Up().Normalized();

	if (newFront.AngleBetweenNorm(float3::float3(0, 1, 0)) > 0.1f && newFront.AngleBetweenNorm(float3::float3(0, -1, 0)) > 0.1f) {	// Limit angle from vertical to 0.1rad //BUG: repeatedly rotating with the mouse breacks the condition!
		frustum.SetFront(newFront.Normalized());
		frustum.SetUp(newUp.Normalized());
	}
}

void ModuleCamera::Yaw(float _speed) {
	float3x3	rotationMatrix = float3x3::RotateY(_speed);
	vec			oldFront = frustum.Front().Normalized();
	vec			oldUp = frustum.Up().Normalized();

	frustum.SetFront((rotationMatrix * oldFront).Normalized());
	frustum.SetUp((rotationMatrix * oldUp).Normalized());
}

void ModuleCamera::Orbit(float _speedFront, float _speedUp, float3 focusPos) {
	// make speed proportional to the distance to the object
	int x = (int)focusPos.x - (int)frustum.Pos().x;
	int y = (int)focusPos.y - (int)frustum.Pos().y;
	int z = (int)focusPos.z - (int)frustum.Pos().z;

	float distanceToFocus = sqrt((float)(x * x + y * y + z * z));

	// move in Frustrum.Up
	MoveUpAxis(_speedUp* distanceToFocus);
	// move in Frustrum.Right
	MoveRightAxis(_speedFront* distanceToFocus);
	
	LookAt((int)focusPos.x, (int)focusPos.y, (int)focusPos.z); // BUG: fast mouse moves make camera go away, because we move on the axis and then look there. We should apply rotation with the movement along!
}

#pragma endregion