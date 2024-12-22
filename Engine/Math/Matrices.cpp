#include "GMath.h"

Matrix4x4::Matrix4x4()
{
	memset(m, 0, 64);
	a[0][0] = a[1][1] = a[2][2] = a[3][3] = 1;
}
Matrix4x4::Matrix4x4(const Matrix4x4& _other) { memcpy(m, _other.m, 64); }

Matrix4x4::Matrix4x4(float _m1, float _m2, float _m3, float _m4,
	float _m5, float _m6, float _m7, float _m8,
	float _m9, float _m10, float _m11, float _m12,
	float _m13, float _m14, float _m15, float _m16)
{
	m[0] = _m1, m[1] = _m2, m[2] = _m3, m[3] = _m4;
	m[4] = _m5, m[5] = _m6, m[6] = _m7, m[7] = _m8;
	m[8] = _m9, m[9] = _m10, m[10] = _m11, m[11] = _m12;
	m[12] = _m13, m[13] = _m14, m[14] = _m15, m[15] = _m16;
}

Matrix4x4 Matrix4x4::Transpose() const
{
	Matrix4x4 mat;

	mat.m[0] = m[0]; mat.m[1] = m[4]; mat.m[2] = m[8]; mat.m[3] = m[12];
	mat.m[4] = m[1]; mat.m[5] = m[5]; mat.m[6] = m[9]; mat.m[7] = m[13];
	mat.m[8] = m[2]; mat.m[9] = m[6]; mat.m[10] = m[10]; mat.m[11] = m[14];
	mat.m[12] = m[3]; mat.m[13] = m[7]; mat.m[14] = m[11]; mat.m[15] = m[15];

	return mat;
}

Matrix4x4 Matrix4x4::Translation(const Vec3 v)
{
	Matrix4x4 mat;

	mat.a[0][3] = v.x;
	mat.a[1][3] = v.y;
	mat.a[2][3] = v.z;

	return mat;
}

Matrix4x4 Matrix4x4::Scaling(const Vec3 v)
{
	Matrix4x4 mat;

	mat.m[0] = v.x;
	mat.m[5] = v.y;
	mat.m[10] = v.z;

	return mat;
}

Matrix4x4 Matrix4x4::RotateX(float angle)
{
	angle *= RAD;
	float sint = sin(angle), cost = cos(angle);
	Matrix4x4 mat;

	mat.m[5] = mat.m[10] = cost;
	mat.m[6] = -sint;
	mat.m[9] = sint;

	return mat;
}

Matrix4x4 Matrix4x4::RotateY(float angle)
{
	angle *= RAD;
	float sint = sinf(angle), cost = cosf(angle);
	Matrix4x4 mat;

	mat.m[0] = mat.m[10] = cost;
	mat.m[2] = -sint;
	mat.m[8] = sint;

	return mat;
}

Matrix4x4 Matrix4x4::RotateZ(float angle)
{
	angle *= RAD;
	float sint = sinf(angle), cost = cosf(angle);
	Matrix4x4 mat;

	mat.m[0] = mat.m[5] = cost;
	mat.m[1] = -sint;
	mat.m[4] = sint;

	return mat;
}

Matrix4x4 Matrix4x4::PerProject(float _fov, float _aspect, float _near, float _far)
{
	float dFov = DegToRad(_fov); // contains conversion from degree
	float tanHalfFov = tan(dFov / 2.f);

	Matrix4x4 _mat;

	_mat.m[0] = 1.0f / (_aspect * tanHalfFov); //  x scale
	_mat.m[5] = 1.0f / tanHalfFov; // y scale
	_mat.m[10] = -_far / (_far - _near); // z scale

	_mat.m[11] = -(_far * _near) / (_far - _near); // perspective division
	_mat.m[14] = -1.0f; // z axis perspective division
	_mat.m[15] = 0.0f;

	return _mat;
}

Matrix4x4 Matrix4x4::OrthoProject(float _width, float _height, float _near, float _far)
{
	Matrix4x4 _mat;

	_mat.m[0] = 2.0 / _width; //  x scale
	_mat.m[5] = 2.0 / _height; // y scale
	_mat.m[10] = 1.0 / (_far - _near); // z scale

	_mat.m[14] = -_near / (_far - _near); // z translation
	_mat.m[15] = 1.0f;

	return _mat;
}

Matrix4x4 Matrix4x4::LookAt(Vec3 _from, Vec3 _to)
{
	Vec3 forward = _to - _from;
	forward.y = 0;
	if (forward.LengthSq() < 0.0001f)
		return Matrix4x4();
	forward = forward.Normalize();

	Vec3 worldUp = Vec3::up;
	if (fabs(Vec3::Dot(forward, worldUp)) > 0.99f)
		worldUp = Vec3(0, 0, 1);
	Vec3 right = Vec3::Cross(worldUp, forward).Normalize();
	Vec3 up = Vec3::Cross(forward, right);

	Matrix4x4 mat;

	mat.a[0][0] = right.x;
	mat.a[0][1] = right.y;
	mat.a[0][2] = right.z;

	mat.a[1][0] = up.x;
	mat.a[1][1] = up.y;
	mat.a[1][2] = up.z;

	mat.a[2][0] = -forward.x;
	mat.a[2][1] = -forward.y;
	mat.a[2][2] = -forward.z;

	// translation is not needed for lookat matrix
	mat.a[0][3] = -Vec3::Dot(_from, right);
	mat.a[1][3] = -Vec3::Dot(_from, up);
	mat.a[2][3] = Vec3::Dot(_from, forward);

	return mat;
}

Matrix4x4 Matrix4x4::View(Vec3 _pos, Vec3 _forward)
{
	_forward = _forward.Normalize();
	Vec3 right = Vec3::Cross(Vec3(0, 1, 0), _forward);
	Vec3 up = Vec3::Cross(_forward, right);

	Matrix4x4 mat;


	mat.a[0][0] = right.x;
	mat.a[0][1] = right.y;
	mat.a[0][2] = right.z;

	mat.a[1][0] = up.x;
	mat.a[1][1] = up.y;
	mat.a[1][2] = up.z;

	mat.a[2][0] = -_forward.x;
	mat.a[2][1] = -_forward.y;
	mat.a[2][2] = -_forward.z;

	mat.a[0][3] = -Vec3::Dot(_pos, right);
	mat.a[1][3] = -Vec3::Dot(_pos, up);
	mat.a[2][3] = Vec3::Dot(_pos, _forward);

	return mat;
}

Matrix4x4 Matrix4x4::View(Vec3 _pos, Vec3 _forward, Vec3 _right, Vec3 _up)
{
	Matrix4x4 mat;

	mat.a[0][0] = _right.x;
	mat.a[0][1] = _right.y;
	mat.a[0][2] = _right.z;

	mat.a[1][0] = _up.x;
	mat.a[1][1] = _up.y;
	mat.a[1][2] = _up.z;

	mat.a[2][0] = -_forward.x;
	mat.a[2][1] = -_forward.y;
	mat.a[2][2] = -_forward.z;

	mat.a[0][3] = -Vec3::Dot(_pos, _right);
	mat.a[1][3] = -Vec3::Dot(_pos, _up);
	mat.a[2][3] = Vec3::Dot(_pos, _forward);

	return mat;
}

Matrix4x4 Matrix4x4::View(Matrix4x4 _world)
{
	Vec3 right = _world.r1.ToVec3();
	Vec3 up = _world.r2.ToVec3();
	Vec3 forward = _world.r3.ToVec3();
	Vec3 pos = Vec3(_world.m[3], _world.m[7], _world.m[11]);

	Matrix4x4 mat;

	mat.a[0][0] = right.x;
	mat.a[0][1] = right.y;
	mat.a[0][2] = right.z;

	mat.a[1][0] = up.x;
	mat.a[1][1] = up.y;
	mat.a[1][2] = up.z;

	mat.a[2][0] = -forward.x;
	mat.a[2][1] = -forward.y;
	mat.a[2][2] = -forward.z;

	mat.a[0][3] = -Vec3::Dot(pos, right);
	mat.a[1][3] = -Vec3::Dot(pos, up);
	mat.a[2][3] = Vec3::Dot(pos, forward);

	return mat;
}

Matrix4x4 Matrix4x4::World(Vec3 _pos, Vec3 _scale, Vec3 _forward, Vec3 _right, Vec3 _up)
{
	Matrix4x4 mat;

	mat.a[0][0] = _right.x * _scale.x;
	mat.a[0][1] = _right.y * _scale.y;
	mat.a[0][2] = _right.z * _scale.z;

	mat.a[1][0] = _up.x * _scale.x;
	mat.a[1][1] = _up.y * _scale.y;
	mat.a[1][2] = _up.z * _scale.z;

	mat.a[2][0] = _forward.x * _scale.x;
	mat.a[2][1] = _forward.y * _scale.y;
	mat.a[2][2] = _forward.z * _scale.z;

	mat.a[0][3] = _pos.x;
	mat.a[1][3] = _pos.y;
	mat.a[2][3] = _pos.z;

	return mat;
}

Matrix4x4 Matrix4x4::World(Vec3 _pos, Vec3 _scale)
{
	return Translation(_pos) * Scaling(_scale);
}

Vec3 Matrix4x4::MulPoint(const Vec3& v) const
{
	Vec3 v1 = Vec3(
		(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
		(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
		(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
	float w;
	w = m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15];
	w = 1.0f / w;
	return (v1 * w);
}

Vec3 Matrix4x4::MulVec(const Vec3& v)
{
	return Vec3(
		(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
		(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
		(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
}

Matrix4x4 Matrix4x4::Mul(const Matrix4x4& matrix) const
{
	Matrix4x4 ret;

	ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
	ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
	ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
	ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];

	ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
	ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
	ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
	ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];

	ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
	ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
	ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
	ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];

	ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
	ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
	ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
	ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];

	return ret;
}

Matrix4x4 Matrix4x4::Inverse() const
{
	Matrix4x4 inv;
	inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
	inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
	inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
	inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
	inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
	inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
	inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
	inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
	inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
	inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
	inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
	inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
	inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
	inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
	inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
	inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

	float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0) {
		throw std::runtime_error("Matrix is singular and cannot be inverted.");
	}
	det = 1.0 / det;

	for (int i = 0; i < 16; i++)
		inv[i] = inv[i] * det;

	return inv;
}

float& Matrix4x4::operator[](int _index) { return m[_index]; }
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& _other) { return Mul(_other); }
Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& _other) { *this = Mul(_other); return *this; }
std::ostream& operator<<(std::ostream& os, const Matrix4x4& _mat)
{
	os << "[" << _mat.m[0] << "," << _mat.m[1] << "," << _mat.m[2] << "," << _mat.m[3] << "]\n";
	os << "[" << _mat.m[4] << "," << _mat.m[5] << "," << _mat.m[6] << "," << _mat.m[7] << "]\n";
	os << "[" << _mat.m[8] << "," << _mat.m[9] << "," << _mat.m[10] << "," << _mat.m[11] << "]\n";
	os << "[" << _mat.m[12] << "," << _mat.m[13] << "," << _mat.m[14] << "," << _mat.m[15] << "]\n";
	return os;
}

//---------------------------------------------------------------


Matrix3x3::Matrix3x3()
{
	memset(m, 0, 36);
	a[0][0] = a[1][1] = a[2][2] = 1;
}

Matrix3x3::Matrix3x3(const Matrix3x3& _other) { memcpy(m, _other.m, 36); }

Matrix3x3::Matrix3x3(const Matrix4x4& _other)
{
	m[0] = _other.m[0]; m[1] = _other.m[1]; m[2] = _other.m[2];
	m[3] = _other.m[3]; m[4] = _other.m[4]; m[5] = _other.m[5];
	m[6] = _other.m[6]; m[7] = _other.m[7]; m[8] = _other.m[8];
}

Matrix3x3::Matrix3x3(float _m1, float _m2, float _m3,
	float _m4, float _m5, float _m6,
	float _m7, float _m8, float _m9)
{
	m[0] = _m1; m[1] = _m2; m[2] = _m3;
	m[3] = _m4; m[4] = _m5; m[5] = _m6;
	m[6] = _m7; m[7] = _m8; m[8] = _m9;
}

Matrix3x3 Matrix3x3::Transpose() const {
	return Matrix3x3(
		m[0], m[3], m[6],
		m[1], m[4], m[7],
		m[2], m[5], m[8]
	);
}

Vec3 Matrix3x3::MulVec(const Vec3& v) {
	return Vec3(
		a[0][0] * v.x + a[0][1] * v.y + a[0][2] * v.z,
		a[1][0] * v.x + a[1][1] * v.y + a[1][2] * v.z,
		a[2][0] * v.x + a[2][1] * v.y + a[2][2] * v.z
	);
}

Matrix3x3 Matrix3x3::Mul(const Matrix3x3& matrix) const {
	Matrix3x3 result;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result.a[i][j] = 0;
			for (int k = 0; k < 3; ++k) {
				result.a[i][j] += a[i][k] * matrix.a[k][j];
			}
		}
	}
	return result;
}

Matrix3x3 Matrix3x3::Inverse() const {
	float det = a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) -
		a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) +
		a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);

	if (det == 0) {
		throw std::runtime_error("Matrix is singular and cannot be inverted.");
	}

	float invDet = 1.0f / det;

	Matrix3x3 inv;
	inv.a[0][0] = (a[1][1] * a[2][2] - a[1][2] * a[2][1]) * invDet;
	inv.a[0][1] = (a[0][2] * a[2][1] - a[0][1] * a[2][2]) * invDet;
	inv.a[0][2] = (a[0][1] * a[1][2] - a[0][2] * a[1][1]) * invDet;

	inv.a[1][0] = (a[1][2] * a[2][0] - a[1][0] * a[2][2]) * invDet;
	inv.a[1][1] = (a[0][0] * a[2][2] - a[0][2] * a[2][0]) * invDet;
	inv.a[1][2] = (a[0][2] * a[1][0] - a[0][0] * a[1][2]) * invDet;

	inv.a[2][0] = (a[1][0] * a[2][1] - a[1][1] * a[2][0]) * invDet;
	inv.a[2][1] = (a[0][1] * a[2][0] - a[0][0] * a[2][1]) * invDet;
	inv.a[2][2] = (a[0][0] * a[1][1] - a[0][1] * a[1][0]) * invDet;

	return inv;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& _other) {
	return Mul(_other);
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& _other) {
	*this = Mul(_other);
	return *this;
}

float& Matrix3x3::operator[](const int& _index) {
	return m[_index];
}

std::ostream& operator<<(std::ostream& os, const Matrix3x3& _mat) {
	os << "[" << _mat.a[0][0] << "\t" << _mat.a[0][1] << "\t" << _mat.a[0][2] << " \n";
	os << " " << _mat.a[1][0] << "\t" << _mat.a[1][1] << "\t" << _mat.a[1][2] << " \n";
	os << " " << _mat.a[2][0] << "\t" << _mat.a[2][1] << "\t" << _mat.a[2][2] << "]\n";
	return os;
}