#pragma once
#include "Behaviour.h"

class Camera :public Behaviour
{
	Matrix viewMat;
	Matrix projMat;
	Matrix viewProj;

	float moveSpeed;
	float rotSpeed;

	static Camera* instance; // singleton instance

public:
	// contructor
	Camera(Vec2 _size, Vec3 _pos, Vec3 _rot, float _nearPlane = 0.1f, float _farPlane = 100.f);
	// returns singleton instance
	static Camera* GetInstance();

	// for free look
	void Update(float _dt) override;
	// update view and viewProj matrices
	void UpdateMat();
	// return view projection matrix
	Matrix GetViewProjMat() const;
	// return view matrix of this camera
	Matrix GetViewMat() const;
	// return projection matrix of camera
	Matrix GetProjMat() const;

	~Camera();
};
