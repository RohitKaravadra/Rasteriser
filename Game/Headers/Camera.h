#pragma once
#include "Behaviour.h"

class Camera :public Behaviour
{
	Matrix4x4 viewMat;
	Matrix4x4 projMat;
	Matrix4x4 viewProj;

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
	Matrix4x4 GetViewProjMat() const;
	// return view matrix of this camera
	Matrix4x4 GetViewMat() const;

	~Camera();
};
