#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "glmath.h"

#include "Dependencies/MathGeoLib/include/Geometry/Frustum.h"
#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Geometry/LineSegment.h"
#include "Dependencies/MathGeoLib/include/Math/float4x4.h"

class GameObject;

class C_Camera : public Component
{
public:
	C_Camera(GameObject* owner);
	~C_Camera();

	bool			Update() override;
	bool			CleanUp() override;


	float*			GetRawViewMatrix();
	mat4x4			GetViewMatrix();
	float*			GetProjectionMatrix();

	void			UpdateTransform(const float4x4& world, const float4x4& parent_world);
	//FROM OLD MODULE CAMERA
	void			PointAt(const vec3& position, const vec3& reference, bool RotateAroundReference = false);
	void			LookAt(const vec3& Spot);
	void			Move(const vec3& Movement);

	void			ReturnToWorldOrigin();

	void			WASDMovement();									// Translates the camera along XYZ (Right, Up, Forward), which will act as the camera's axis.
	void			FreeLookAround();								// Receives information about the mouse's motion values and rotates the camera on it's axis.
	void			RotateAroundReference();						// Will rotate the camera around the point specified by the reference variable. Almost identical to FreeLookAround();
	void			PanCamera();									// Will translate the camera along XY (Right, Up). The movement can be described as moving in the plane of the camera frame.
	void			Zoom();											// Will translate the camera along the Z (Forward) axis. This means that the camera will move forwards and backwards. 


	void			InitFrustum();

	float			GetFOV() const;
	void			SetFOV(float fov);

	float			GetAspectRatio() const;
	void			SetAspectRatio(float ratio);

	float			GetBackPlane() const;
	void			SetBackPlane(float dist);

	float			GetFrontPlane() const;
	void			SetFrontPlane(float dist);

	vec*			GetFrustumPoints() const;
	bool			FrustumContains(const AABB& refBox) const;


public:
	Frustum		frustum;

	bool		camera_is_active;
	bool		culling_is_active;

	bool		is_looking;
	float3		camera_target;

private:
	LineSegment	ray;

public:
	vec3 X;															// Right vector of the Camera. Will always remain axis aligned in this implementation.
	vec3 Y;															// Up vector of the Camera.
	vec3 Z;															// Forward Vector of the Camera. Represents where the camera is pointing to.

	vec3 position_origin;											// 
	vec3 reference_origin;
	vec3 position;													// Position of the Camera in the World Space.
	vec3 reference;													// Point where the camera will look at and rotate around.

	float movement_speed;											// 
	float rotation_speed;											// 
	float zoom_speed;												// 

};

#endif // !_C_CAMERA_H__
