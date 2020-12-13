#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

class GameObject;

class C_Transform : public Component
{
public:
	C_Transform(GameObject* owner);
	~C_Transform();

	bool Update() override;
	bool CleanUp() override;

public:
	void RecalculateWorldTransform();
	void RecalculateLocalTransform();
	
	float3 GetPosition() const;
	float3 GetRotation() const;
	float3 GetScale() const;

	float4x4 GetLocalTransform();
	float4x4 GetWorldTransform();
	
	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation);
	void SetScale(const float3& scale);

	void SetTransform(const float3& new_position, const Quat& new_rotation, const float3& new_scale);
	void AddTransform(const float3& vec3);

	void SetLocalTransform(float3 position, float3 scale, Quat rotation);
	void SetWorldTransform(float4x4 new_transform);



public:
	float4x4	matrix;
	
	bool		recalculate_world_transform;			// Set to true if the parent of the owner object has been changed. That would mean recalculating the local position.

private:
	float3		world_position;
	Quat		world_rotation;
	float3		world_scale;

	float3		local_euler_rotation;

	float3		local_position;
	Quat		local_rotation;
	float3		local_scale;

	float4x4	local_transform;
	float4x4	world_transform;
};

#endif // !_C_TRANSFORM_H__