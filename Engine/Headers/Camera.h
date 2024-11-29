#pragma once
#include "Behaviour.h"

class Camera :public Behaviour
{
	Matrix viewMat;
	Matrix projMat;
	Matrix viewProj;

	// update view and viewProj matrices
	void UpdateMat();
public:
	// contructor
	Camera(Vec2 _size, Vec3 _pos, Vec3 _rot);

	void OnWorldUpdate() override;

	Matrix GetViewProjMat() const;
};