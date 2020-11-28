#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Globals.h"
#include "Module.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"

class ModuleCamera : public Module
{
public:

	Frustum	frustum = Frustum::Frustum();

private:

	float4x4		projectionMatrix	= float4x4::identity;	// Projection Matrix
	float4x4		viewMatrix			= float4x4::identity;	// View Matrix
	float			mTicks		= 0;
	unsigned int	mTicksCount = 0;		// Tick counter for deltaTime
	float			deltaTime	= 0;

public:
	
	ModuleCamera();
	~ModuleCamera();

	// ----------- Module Functions ---------- //
	bool			Init();
	update_status	PreUpdate();
	update_status	Update();
	update_status	PostUpdate();
	bool			CleanUp();

	// ---------- Getters & Setters ---------- //
	float4x4	GetProjectionMatrix() const	{ return this->projectionMatrix; };
	float4x4	GetViewMatrix() const		{ return this->viewMatrix; };
	vec			GetPosition() const			{ return frustum.Pos(); }
	vec			GetOrientation() const		{ return frustum.Front(); }

	void		SetPosition(float _x, float _y, float _z)		{ frustum.SetPos(float3::float3(_x, _y, _z)); }
	void		SetPlaneDistances(float _zNear, float _zFar)	{ frustum.SetViewPlaneDistances(_zNear, _zFar); }
	void		SetFOV(unsigned int width, unsigned int height);	// set the horizontal FOV keeping the aspect ratio
	//void		SetAspectRatio();									// change the vertical FOV to meet the new aspect ratio.
 
	// ------------ Module Camera ------------ //
	void LookAt(int _x, int _y, int _z);
	
private:
	// ---------- Camera Traslations --------- //
	void MoveFrontAxis(float _speed);
	void MoveRightAxis(float _speed);
	void MoveUpAxis(float _speed);
	void MoveYAxisUnit(int upOrDown);								 // Yaws in the absolute Y axis, its less disorienting

	// ----------- Camera Rotations ---------- //
	void Pitch(float _speed);
	void Yaw(float _speed);
	void Orbit(float _speedFront, float _speedUp, float3 focusPos);

};
#endif // __ModuleCamera_H__
