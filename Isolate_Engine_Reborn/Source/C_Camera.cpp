#include "Globals.h"
#include "glew/include/glew.h"
#include "Color.h"

#include "Application.h"
#include "M_Window.h"
#include "M_Input.h"
#include "M_Renderer3D.h"

#include "GameObject.h"

#include "C_Camera.h"
#include "C_Transform.h"

C_Camera::C_Camera(GameObject* owner) : Component(owner, COMPONENT_TYPE::CAMERA, "Camera"),
camera_is_active(false),
culling_is_active(false)
{
	InitFrustum();
}

C_Camera::~C_Camera()
{}

bool C_Camera::Update()
{
	if (owner != nullptr)
	{
		owner->DrawFrustumBox();
	}
	return true;
}

bool C_Camera::CleanUp()
{


	return true;
}

void C_Camera::InitFrustum()
{
	frustum.SetUp(float3::unitY);
	frustum.SetFront(float3::unitZ);

	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	frustum.SetPos(float3(0, 0, 0));
	frustum.SetViewPlaneDistances(1.0f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	if (owner != nullptr)
	{
		C_Transform* transform = (C_Transform*)owner->GetComponent(COMPONENT_TYPE::TRANSFORM);
		transform->RecalculateLocalTransform();
	}

	SetAspectRatio((float)App->window->GetWidth() / (float)App->window->GetHeight());
	SetFOV(30.0f);
}

void C_Camera::UpdateTransform(const float4x4& world, const float4x4& parent_world)
{
	frustum.SetFront(world.WorldZ());
	frustum.SetUp(world.WorldY());

	Quat   rot = Quat::identity;
	float3 scale = {1,1,1};
	float3 pos = {0,0,0};

	world.Decompose(pos, rot, scale);

	frustum.SetPos(pos);
}

float* C_Camera::GetRawViewMatrix()//
{
	static float4x4 frustum_mat = frustum.ViewMatrix();
	frustum_mat.Transpose();

	return (float*)frustum_mat.v;
}


float* C_Camera::GetProjectionMatrix()//
{
	static float4x4 frustum_mat = frustum.ProjectionMatrix();
	frustum_mat.Transpose();

	return (float*)frustum_mat.v;
}

mat4x4 C_Camera::GetViewMatrix()
{
	static float4x4 frustum_mat = frustum.ViewMatrix();

	frustum_mat.Transpose();

	float3 pos = {0,0,0};
	float3 scale = {1,1,1};
	Quat   rot = Quat::identity;

	frustum_mat.Decompose(pos, rot, scale);

	float3 rot_axis;
	float rot_angle;

	rot.ToAxisAngle(rot_axis, rot_angle);

	vec3 rot_axis_v3 = {rot_axis.x, rot_axis.y, rot_axis.z};

	mat4x4 view_mat;
	view_mat.translate(pos.x, pos.y, pos.z);
	view_mat.rotate(rot_angle, rot_axis_v3);
	view_mat.scale(scale.x, scale.y, scale.z);

	return view_mat;
}


float C_Camera::GetFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

void C_Camera::SetFOV(float fov)
{
	frustum.SetVerticalFovAndAspectRatio(fov * DEGTORAD, ((float)App->window->GetWidth() / (float)App->window->GetHeight()));
}

float C_Camera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void C_Camera::SetAspectRatio(float ratio) {

	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), ratio);
}

float C_Camera::GetBackPlane() const
{
	return frustum.NearPlaneDistance();
}

void C_Camera::SetBackPlane(float dist)
{
	if (dist > 0.0f && dist < frustum.FarPlaneDistance())
	{
		frustum.SetViewPlaneDistances(dist, frustum.FarPlaneDistance());
	}
}

float C_Camera::GetFrontPlane() const
{
	return frustum.FarPlaneDistance();
}

void C_Camera::SetFrontPlane(float dist)
{
	if (dist > 0.0f && dist > frustum.NearPlaneDistance())
	{
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), dist);
	}
}

vec* C_Camera::GetFrustumPoints() const
{
	static vec frustum_edges[8];

	frustum.GetCornerPoints(frustum_edges);

	return frustum_edges;
}

//
//void C_Camera::WASDMovement()
//{
//	vec3 new_position(0, 0, 0);
//	float mov_speed = movement_speed * App->GetUnpausableDt();
//
//	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
//	{
//		mov_speed = movement_speed * 2 * App->GetUnpausableDt();
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)
//	{
//		new_position -= Z * mov_speed;
//	}
//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)
//	{
//		new_position += Z * mov_speed;
//	}
//
//
//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)
//	{
//		new_position -= X * mov_speed;
//	}
//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)
//	{
//		new_position += X * mov_speed;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_STATE::KEY_REPEAT)
//	{
//		new_position += Y * mov_speed;
//	}
//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_REPEAT)
//	{
//		new_position -= Y * mov_speed;
//	}
//
//	Move(new_position);
//}

