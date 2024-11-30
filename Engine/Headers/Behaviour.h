#pragma once
#include "GMath.h"
#include "Driver.h"

struct Transform
{
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

	Vec3 forward;
	Vec3 right;
	Vec3 up;

	Matrix worldMat;

	Transform();
	Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale);
	void SetMat();
	friend std::ostream& operator<<(std::ostream& os, const Transform& transform);
};

class StaticBehaviour
{
protected:
	Transform transform;

	StaticBehaviour(Vec3 _pos = Vec3::zero, Vec3 _rot = Vec3::zero, Vec3 _scale = Vec3::one);
public:
	Vec3 GetPos() const;
	Vec3 GetRot() const;
	Vec3 GetScale() const;

	Vec3 Forward() const;
	Vec3 Right() const;
	Vec3 Up() const;

	virtual void Update(float _dt) {};
	virtual void Draw() {};
};

class Behaviour :public StaticBehaviour
{
protected:
	Behaviour(Vec3 _pos = Vec3::zero, Vec3 _rot = Vec3::zero, Vec3 _scale = Vec3::one);
public:

	void Position(Vec3 _pos);
	void Rotation(Vec3 _rot);
	void Scale(Vec3 _scale);

	void Translate(Vec3 _vel);
	void TranslateRel(Vec3 _vel);
	void Rotate(Vec3 _angle);

	virtual void OnWorldUpdate() {};
};
