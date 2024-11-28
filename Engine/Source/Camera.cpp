#include "Camera.h"


Camera::Camera(Vec2 _size, Vec3 _pos, Vec3 _rot)
{
	pos = _pos;
	rot = _rot;

	projMat = Matrix::Projection(45, _size.x / _size.y, 0.1f, 200.f);
	UpdateRotation();
}

void Camera::UpdateRotation()
{
	Quaternion qRot = Quaternion::FromEuler(rot);
	forward = qRot * Vec3(0, 0, -1);
	right = qRot * Vec3(1, 0, 0);
	up = qRot * Vec3(0, 1, 0);

	UpdateMat();
}

void Camera::UpdateMat()
{
	viewMat = Matrix::View(pos, forward, right, up);
	viewProj = projMat * viewMat;
}

void Camera::SetRotation(float _horz, float _vert)
{
	rot = Vec3(_vert, _horz, 0);

	UpdateRotation();
}

void Camera::SetPos(Vec3 _pos)
{
	pos = _pos;

	UpdateMat();
}

void Camera::Rotate(float _horz, float _vert)
{
	rot += Vec3(_vert, _horz, 0);
	rot %= 360.f;
	UpdateRotation();
}

void Camera::Move(Vec3 _vel)
{
	pos += forward * _vel.z + right * _vel.x + up * _vel.y;

	UpdateMat();
}

Vec3 Camera::Pos() const { return pos; }
Vec3 Camera::Forward() const { return forward; }
Matrix Camera::ViewMat() const { return viewMat; }
Matrix Camera::GetViewProjMat() const { return viewProj; }