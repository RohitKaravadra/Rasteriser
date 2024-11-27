#pragma once
#include "GMath.h"

class Camera
{
	SphericalCoordinate sphRot;
	Vec3 pos;
	Vec3 forward;
	Matrix viewMat;
public:
	// contructor
	Camera(Vec3 _pos, Vec3 _forward);
	// set camera rotation
	void SetRotation(float _horz, float _vert);
	// set camera position
	void SetPos(Vec3 _pos);
	// rotate canera with given angles in degree
	void Rotate(float _horz, float _vert);
	// move camera with given velocity
	void Move(Vec3 _vel);

	Vec3 Pos() const;
	Vec3 Forward() const;
	Matrix ViewMat() const;
};