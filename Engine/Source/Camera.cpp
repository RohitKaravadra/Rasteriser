#include "Camera.h"


Camera::Camera(Vec2 _size, Vec3 _pos, Vec3 _rot, float _nearPlane, float _farPlane) :Behaviour(_pos, _rot)
{
	projMat = Matrix::Projection(45, _size.x / _size.y, _nearPlane, _farPlane);
	UpdateMat();
}

void Camera::UpdateMat()
{
	viewMat = Matrix::View(transform.worldMat);
	viewProj = projMat * viewMat;
}

void Camera::OnWorldUpdate()
{
	UpdateMat();
}

Matrix Camera::GetViewProjMat() const { return viewProj; }