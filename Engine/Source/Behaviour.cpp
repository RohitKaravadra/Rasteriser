#include "Behaviour.h"

void Behaviour::OnScale()
{
	worldMat.a[0][0] = right.x * scale.x;
	worldMat.a[0][1] = right.y * scale.y;
	worldMat.a[0][2] = right.z * scale.z;

	worldMat.a[1][0] = up.x * scale.x;
	worldMat.a[1][1] = up.y * scale.y;
	worldMat.a[1][2] = up.z * scale.z;

	worldMat.a[2][0] = forward.x * scale.x;
	worldMat.a[2][1] = forward.y * scale.y;
	worldMat.a[2][2] = forward.z * scale.z;

	OnWorldUpdate();
}

void Behaviour::OnRotate()
{
	Quaternion qRot = Quaternion::FromEuler(rot);
	forward = qRot * Vec3(0, 0, -1);
	right = qRot * Vec3(1, 0, 0);
	up = qRot * Vec3(0, 1, 0);

	OnScale();
}

void Behaviour::OnTranslate()
{
	worldMat.a[0][3] = pos.x;
	worldMat.a[1][3] = pos.y;
	worldMat.a[2][3] = pos.z;

	OnWorldUpdate();
}

Behaviour::Behaviour(Vec3 _pos, Vec3 _rot, Vec3 _scale) :pos(_pos), rot(_rot), scale(_scale)
{
	OnRotate();
	OnTranslate();
}

Vec3 Behaviour::Position() const { return pos; }
Vec3 Behaviour::Rotation() const { return rot; }
Vec3 Behaviour::Scale() const { return scale; }
Vec3 Behaviour::Forward() const { return forward; };
Vec3 Behaviour::Right() const { return right; };
Vec3 Behaviour::Up() const { return up; };

void Behaviour::Position(Vec3 _pos)
{
	pos = _pos;
	OnTranslate();
}

void Behaviour::Rotation(Vec3 _rot)
{
	rot = _rot % 360.f;
	OnRotate();
}

void Behaviour::Scale(Vec3 _scale)
{
	scale = scale;
	OnScale();
}

void Behaviour::Translate(Vec3 _vel)
{
	pos += _vel;
	OnTranslate();
}

void Behaviour::TranslateRel(Vec3 _vel)
{
	pos += right * _vel.x + up * _vel.y + forward * _vel.z;
	OnTranslate();
}

void Behaviour::Rotate(Vec3 _angle)
{
	rot += _angle;
	rot %= 360.f;
	OnRotate();
}

StaticBehaviour::StaticBehaviour(Vec3 _pos, Vec3 _rot, Vec3 _scale)
{
	Quaternion qRot = Quaternion::FromEuler(rot);
	forward = qRot * Vec3(0, 0, -1);
	right = qRot * Vec3(1, 0, 0);
	up = qRot * Vec3(0, 1, 0);

	worldMat.a[0][0] = right.x * scale.x;
	worldMat.a[0][1] = right.y * scale.y;
	worldMat.a[0][2] = right.z * scale.z;

	worldMat.a[1][0] = up.x * scale.x;
	worldMat.a[1][1] = up.y * scale.y;
	worldMat.a[1][2] = up.z * scale.z;

	worldMat.a[2][0] = forward.x * scale.x;
	worldMat.a[2][1] = forward.y * scale.y;
	worldMat.a[2][2] = forward.z * scale.z;

	worldMat.a[0][3] = pos.x;
	worldMat.a[1][3] = pos.y;
	worldMat.a[2][3] = pos.z;
}

Vec3 StaticBehaviour::Position() const { return pos; }
Vec3 StaticBehaviour::Rotation() const { return rot; }
Vec3 StaticBehaviour::Scale() const { return scale; }
Vec3 StaticBehaviour::Forward() const { return forward; };
Vec3 StaticBehaviour::Right() const { return right; };
Vec3 StaticBehaviour::Up() const { return up; };