#include "Camera.h"


Camera::Camera(Vec2 _size, Vec3 _pos, Vec3 _rot) :Behaviour(_pos, _rot)
{
	projMat = Matrix::Projection(45, _size.x / _size.y, 0.1f, 1000.f);
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