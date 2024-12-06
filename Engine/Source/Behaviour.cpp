#include "Behaviour.h"

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