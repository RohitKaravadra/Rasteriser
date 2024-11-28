#pragma once
#include "GMath.h"

class Camera
{
	Vec3 pos;
	Vec3 rot;

	Vec3 forward;
	Vec3 right;
	Vec3 up;

	Matrix viewMat;
	Matrix projMat;
	Matrix viewProj;

	// update all rotation related data
	void UpdateRotation();
	// update view and viewProj matrices
	void UpdateMat();
public:
	// contructor
	Camera(Vec2 _size, Vec3 _pos, Vec3 _rot);

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
	Matrix GetViewProjMat() const;
};