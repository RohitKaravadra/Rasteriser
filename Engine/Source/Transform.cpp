#include "Transform.h"

Transform::Transform()
{
	position = Vec3::zero;
	scale = Vec3::one;
	rotation = Vec3::zero;

	worldMat = Matrix4x4();
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

	worldMat = Matrix4x4::World(position, scale, forward, right, up);
}
