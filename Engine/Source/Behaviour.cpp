#include "Behaviour.h"

Transform::Transform()
{
	position = Vec3::zero;
	scale = Vec3::one;
	rotation = Vec3::zero;

	worldMat = Matrix::Identity();
}

Transform::Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	position = _pos;
	scale = _scale;
	rotation = _rot;

	Update();
}

void Transform::Update()
{
	Quaternion qRot = Quaternion::FromEuler(rotation);

	forward = qRot * Vec3::front;
	right = qRot * Vec3::right;
	up = qRot * Vec3::up;

	worldMat = Matrix::World(position, scale, forward, right, up);
}

std::ostream& operator<<(std::ostream& os, const Transform& _t)
{
	return os << _t.worldMat << "\n";
}

Behaviour::Behaviour(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	transform = Transform(_pos, _rot, _scale);
}

void Behaviour::Translate(Vec3 _vel)
{
	transform.position += _vel;
	transform.Update();
}

void Behaviour::TranslateRel(Vec3 _vel)
{
	transform.position += transform.right * _vel.x + transform.up * _vel.y + transform.forward * _vel.z;
	transform.Update();
}

void Behaviour::Rotate(Vec3 _angle)
{
	transform.rotation += _angle;
	transform.rotation %= 360.f;
	transform.Update();
}