#include "GameObject.h"

#include "C_Camera.h"

C_Camera::C_Camera(GameObject* owner) : Component(owner, COMPONENT_TYPE::CAMERA, "Camera")
{

}

C_Camera::~C_Camera()
{}

bool C_Camera::Update()
{

	return true;
}

bool C_Camera::CleanUp()
{


	return true;
}