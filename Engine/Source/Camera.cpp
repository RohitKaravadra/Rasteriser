#include "Camera.h"


Camera::Camera(Vec3 _pos, Vec3 _forward)
{
	pos = _pos;
	forward = _forward.Normalize();
	sphRot = SphericalCoordinate::fromCartesian(forward);
	viewMat = Matrix::View(pos, forward);
}

void Camera::SetRotation(float _horz, float _vert)
{
	sphRot.theta = DegToRad(_vert);
	sphRot.phi = DegToRad(_horz);
	sphRot.Normalize();
	viewMat = Matrix::View(pos, forward);
}

void Camera::SetPos(Vec3 _pos)
{
	pos = _pos;
	viewMat = Matrix::View(pos, forward);
}

void Camera::Rotate(float _horz, float _vert)
{
	sphRot.Rotate(_horz, _vert);
	forward = sphRot.ToCartesian().Normalize();
	viewMat = Matrix::View(pos, forward);
}

void Camera::Move(Vec3 _vel)
{
	pos += forward * _vel.z;
	viewMat = Matrix::View(pos, forward);
}

Vec3 Camera::Pos() const { return pos; }
Vec3 Camera::Forward() const { return forward; }
Matrix Camera::ViewMat() const { return viewMat; }