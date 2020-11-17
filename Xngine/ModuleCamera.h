#pragma once
#include "Globals.h"
#include "Module.h"
#include "Geometry/Frustum.h"

class ModuleCamera : public Module
{

public:
	// Module Functions
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	// Camera Settings
	bool SetFOV(unsigned width, unsigned height);			// should set the horizontal FOV keeping the aspect ratio
	bool SetAspectRatio();									// should change the vertical FOV to meet the new aspect ratio.
 
	bool SetPlaneDistances(float _zNear, float _zFar);
	bool LookAt(int _x, int _y, int _z);
	vec Position() {
		return frustum.Pos();
	}
	vec Orientation() {
		return frustum.Front();
	}

	bool SetPosition(float _x, float _y, float _z) {
		frustum.SetPos(float3::float3(_x, _y, _z));
	}
	

	float4x4 GetProjectionMatrix() {
		return this->projectionGL;
	}; 
	float4x4 GetViewMatrix() {
		return this->viewMatrix;
	};

private:
	Frustum frustum;
	float4x4 projectionGL;		// Projection Matrix
	float4x4 viewMatrix;		// View Matrix

	unsigned int mTicksCount; // Tick counter for deltaTime

private:
	// Camera Traslations
	bool MoveFrontAxis(float _speed);
	bool MoveRightAxis(float _speed);
	bool MoveYAxis(int upOrDown);
	// Camera Rotations
	bool Pitch(float _speed);
	bool Yaw(float _speed);

};

