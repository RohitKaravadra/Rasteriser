#include "Behaviour.h"

Transform::Transform()
{
	pos = Vec3::zero;
	scale = Vec3::one;
	rot = Vec3::zero;

	worldMat = Matrix::Identity();
}

Transform::Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	pos = _pos;
	scale = _scale;
	rot = _rot;

	SetMat();
}

void Transform::SetMat()
{
	Quaternion qRot = Quaternion::FromEuler(rot);

	forward = qRot * Vec3::front;
	right = qRot * Vec3::right;
	up = qRot * Vec3::up;

	worldMat = Matrix::World(pos, scale, forward, right, up);
}

std::ostream& operator<<(std::ostream& os, const Transform& _t)
{
	return os << _t.worldMat << "\n";
}

StaticBehaviour::StaticBehaviour(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	transform = Transform(_pos, _rot, _scale);
}

Vec3 StaticBehaviour::GetPos() const { return transform.pos; }
Vec3 StaticBehaviour::GetRot() const { return transform.rot; }
Vec3 StaticBehaviour::GetScale() const { return transform.scale; }
Vec3 StaticBehaviour::Forward() const { return transform.forward; };
Vec3 StaticBehaviour::Right() const { return transform.right; };
Vec3 StaticBehaviour::Up() const { return transform.up; };

Behaviour::Behaviour(Vec3 _pos, Vec3 _rot, Vec3 _scale) :StaticBehaviour(_pos, _rot, _scale)
{
}

void Behaviour::Position(Vec3 _pos)
{
	transform.pos = _pos;
	transform.SetMat();
	OnWorldUpdate();
}

void Behaviour::Rotation(Vec3 _rot)
{
	transform.rot = _rot;
	transform.SetMat();
	OnWorldUpdate();
}

void Behaviour::Scale(Vec3 _scale)
{
	transform.scale = _scale;
	transform.SetMat();
	OnWorldUpdate();
}

void Behaviour::Translate(Vec3 _vel)
{
	transform.pos += _vel;
	transform.SetMat();
	OnWorldUpdate();
}

void Behaviour::TranslateRel(Vec3 _vel)
{
	transform.pos += transform.right * _vel.x + transform.up * _vel.y + transform.forward * _vel.z;
	transform.SetMat();
	OnWorldUpdate();
}

void Behaviour::Rotate(Vec3 _angle)
{
	transform.rot += _angle;
	transform.rot %= 360.f;
	transform.SetMat();
	OnWorldUpdate();
}