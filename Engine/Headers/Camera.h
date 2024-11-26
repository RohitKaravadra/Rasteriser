#pragma once
#include "GMath.h"

class Camera
{
	SphericalCoordinate sphRot;
	Vec3 pos;
	Vec3 forward;
	Matrix viewMat;
public:

	Camera(Vec3 _pos, Vec3 _forward)
	{
		pos = _pos;
		forward = _forward.Normalize();
		sphRot = SphericalCoordinate::fromCartesian(forward);
		viewMat = Matrix::View(pos, forward);
	}

	void SetRotation(float _horz, float _vert)
	{
		sphRot.theta = DegToRad(_vert);
		sphRot.phi = DegToRad(_horz);
		sphRot.Normalize();
		viewMat = Matrix::View(pos, forward);
	}

	void SetPos(Vec3 _pos)
	{
		pos = _pos;
		viewMat = Matrix::View(pos, forward);
	}

	void Rotate(float _horz, float _vert)
	{
		sphRot.Rotate(_horz, _vert);
		forward = sphRot.toCartesian().Normalize();
		viewMat = Matrix::View(pos, forward);
	}

	void Move(Vec3 _delta)
	{
		pos += forward * _delta.z;
		viewMat = Matrix::View(pos, forward);
	}

	Vec3 Pos() { return pos; }
	Vec3 Forward() { return forward; }
	Matrix ViewMat() { return viewMat; }
};