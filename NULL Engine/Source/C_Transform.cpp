#include "MathGeoLib/src/Math/TransformOps.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
matrix(matrix.identity),
recalculate_world_transform(false)
{

	local_position = float3(0.f, 0.f, 0.f);
	local_scale = float3(1.f, 1.f, 1.f);

	euler_rotation = float3(0.f, 0.f, 0.f);
	local_rotation = Quat::identity;

	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);
	world_transform = Quat::identity;
}

C_Transform::~C_Transform()
{

}

bool C_Transform::Update()
{
	bool ret = true;



	return ret;
}

bool C_Transform::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- C_TRANSFORM METHODS ---
float3 C_Transform::GetPosition() const
{
	return local_position;
}

float3 C_Transform::GetRotation() const
{
	return euler_rotation;
}

float3 C_Transform::GetScale() const
{
	return local_scale;
}

void C_Transform::SetPosition(const float3& position)
{
	this->local_position = position;

	RecalculateLocalTransform();
}

void C_Transform::SetRotation(const float3& rotation)
{
	euler_rotation = rotation;

	//this->local_rotation.RotateX(euler_rotation.x);
	//this->local_rotation.RotateY(euler_rotation.y);
	//this->local_rotation.RotateZ(euler_rotation.z);

	RecalculateLocalTransform();
}

void C_Transform::SetScale(const float3& scale)
{
	this->local_scale = scale;

	RecalculateLocalTransform();
}

void C_Transform::SetLocalTransform(float3 position, float3 scale, Quat rotation)
{
	this->local_position = position;
	this->local_scale = scale;
	this->local_rotation = rotation;

	recalculate_world_transform = true;
}

float4x4 C_Transform::GetLocalTransform()
{
	return local_transform;
}

float4x4 C_Transform::GetWorldTransform()
{
	return world_transform;
}

void C_Transform::RecalculateWorldTransform()
{
	if (owner->parent != nullptr)
	{
		world_transform = owner->parent->GetTransform()->world_transform * local_transform;
	}
	else
	{
		world_transform = local_transform;
	}

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		owner->childs[i]->GetTransform()->RecalculateWorldTransform();
	}
}

void C_Transform::RecalculateLocalTransform()
{
	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);

	RecalculateWorldTransform();
}