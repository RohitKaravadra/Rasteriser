#include <GMath.h>
#include <iostream>

#pragma region Vec2 

//------------------------------------------------------------------------------------------------

Vec2 Vec2::operator+(const Vec2& _v) const { return Vec2(x + _v.x, y + _v.y); }
Vec2 Vec2::operator-(const Vec2& _v) const { return Vec2(x - _v.x, y - _v.y); }
Vec2 Vec2::operator*(const Vec2& _v) const { return Vec2(x * _v.x, y * _v.y); }
Vec2 Vec2::operator*(float _val) const { return Vec2(x * _val, y * _val); }
Vec2 Vec2::operator/(float _val) const { return Vec2(x / _val, y / _val); }
Vec2 Vec2::operator%(float _val) const { return Vec2(fmod(x, _val), fmod(y, _val)); }
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }

Vec2& Vec2::operator+=(const Vec2& _v) { x += _v.x; y += _v.y; return *this; }
Vec2& Vec2::operator-=(const Vec2& _v) { x -= _v.x; y -= _v.y; return *this; }
Vec2& Vec2::operator*=(const Vec2& _v) { x *= _v.x; y *= _v.y; return *this; }
Vec2& Vec2::operator*=(float _val) { x *= _val; y *= _val; return *this; }
Vec2& Vec2::operator/=(float _val) { x /= _val; y /= _val; return *this; }
Vec2& Vec2::operator%=(float _val) { x = fmod(x, _val); y = fmod(y, _val); return *this; }

float Vec2::LengthSq(void) const { return SQ(x) + SQ(y); }
float Vec2::Length(void) const { return sqrtf(LengthSq()); }

Vec2 Vec2::Normalize(void)
{
	float len = Length();
	return len > 0 ? *this / len : *this;
}

float Vec2::NormalizeGetLength(void)
{
	float len = Length();
	if (len > 0)
		*this /= len;
	return len;
}

float Vec2::Max() const { return max(x, y); }
float Vec2::Min() const { return min(x, y); }

Vec2 Vec2::Max(const Vec2& _v1, const Vec2& _v2) { return Vec2(max(_v1.x, _v2.x), max(_v1.y, _v2.y)); }
Vec2 Vec2::Min(const Vec2& _v1, const Vec2& _v2) { return Vec2(min(_v1.x, _v2.x), min(_v1.y, _v2.y)); }

float Vec2::Dot(const Vec2& _v1, const Vec2& _v2) { return _v1.x * _v2.x + _v1.y * _v2.y; }
float Vec2::Cross(const Vec2& _v1, const Vec2& _v2) { return _v1.x * _v2.y - _v1.y * _v2.x; }

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Vec3 

//------------------------------------------------------------------------------------------------

Vec3 Vec3::zero(0, 0, 0);
Vec3 Vec3::one(1, 1, 1);
Vec3 Vec3::up(0, 1, 0);
Vec3 Vec3::down(0, -1, 0);
Vec3 Vec3::right(1, 0, 0);
Vec3 Vec3::left(-1, 0, 0);
Vec3 Vec3::front(0, 0, -1);
Vec3 Vec3::back(0, 0, 1);

Vec3 Vec3::operator+(const Vec3& _v) const { return Vec3(x + _v.x, y + _v.y, z + _v.z); }
Vec3 Vec3::operator-(const Vec3& _v) const { return Vec3(x - _v.x, y - _v.y, z - _v.z); }
Vec3 Vec3::operator*(const Vec3& _v) const { return Vec3(x * _v.x, y * _v.y, z * _v.z); }
Vec3 Vec3::operator*(float _val) const { return Vec3(x * _val, y * _val, z * _val); }
Vec3 Vec3::operator/(float _val) const { return Vec3(x / _val, y / _val, z / _val); }
Vec3 Vec3::operator%(float _val) const { return Vec3(std::fmod(x, _val), std::fmod(y, _val), std::fmod(z, _val)); }
Vec3 Vec3::operator-() const { return Vec3(-x, -y, -z); }

Vec3& Vec3::operator+=(const Vec3& _v) { x += _v.x; y += _v.y; z += _v.z; return *this; }
Vec3& Vec3::operator-=(const Vec3& _v) { x -= _v.x; y -= _v.y; z -= _v.z; return *this; }
Vec3& Vec3::operator*=(const Vec3& _v) { x *= _v.x; y *= _v.y; z *= _v.z; return *this; }
Vec3& Vec3::operator*=(float _val) { x *= _val; y *= _val; z *= _val; return *this; }
Vec3& Vec3::operator/=(float _val) { x /= _val; y /= _val; z /= _val; return *this; }
Vec3 Vec3::operator%=(float _val) { x = std::fmod(x, _val); y = std::fmod(y, _val); z = std::fmod(z, _val); return *this; }

float Vec3::LengthSq(void) const { return SQ(x) + SQ(y) + SQ(z); }
float Vec3::Length(void) const { return sqrtf(LengthSq()); }

Vec3 Vec3::Normalize() const
{
	float len = Length();
	return len > 0 ? Vec3(x / len, y / len, z / len) : Vec3(x, y, z);
}

float Vec3::NormalizeGetLength(void)
{
	float len = Length();
	if (len > 0)
		*this /= len;
	return len;
}

Vec3 Vec3::Abs() { return Vec3(std::abs(x), std::abs(y), std::abs(z)); }

float Vec3::Dot(const Vec3& _v1, const Vec3& _v2) {
	return _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z;
}

Vec3 Vec3::Cross(const Vec3& _v1, const Vec3& _v2) {
	return Vec3(_v1.y * _v2.z - _v1.z * _v2.y,
		_v1.z * _v2.x - _v1.x * _v2.z,
		_v1.x * _v2.y - _v1.y * _v2.x);
}

float Vec3::Max(void) const { return max(x, max(y, z)); }
float Vec3::Min(void) const { return min(x, min(y, z)); }

Vec3 Vec3::Max(const Vec3& v1, const Vec3& v2)
{
	return Vec3(max(v1.x, v2.x),
		max(v1.y, v2.y),
		max(v1.z, v2.z));
}

Vec3 Vec3::Min(const Vec3& v1, const Vec3& v2)
{
	return Vec3(min(v1.x, v2.x),
		min(v1.y, v2.y),
		min(v1.z, v2.z));
}

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Vec4

//------------------------------------------------------------------------------------------------

Vec4 Vec4::operator+(const Vec4& _v) const { return Vec4(x + _v.x, y + _v.y, z + _v.z, _v.w + w); }
Vec4 Vec4::operator-(const Vec4& _v) const { return Vec4(x - _v.x, y - _v.y, z - _v.z, _v.w - w); }
Vec4 Vec4::operator*(const Vec4& _v) const { return Vec4(x * _v.x, y * _v.y, z * _v.z, _v.w * w); }
Vec4 Vec4::operator*(float _val) const { return Vec4(x * _val, y * _val, z * _val, w * _val); }
Vec4 Vec4::operator/(float _val) const { return Vec4(x / _val, y / _val, z / _val, w / _val); }
Vec4 Vec4::operator-() const { return Vec4(-x, -y, -z, -w); }

Vec4& Vec4::operator+=(const Vec4& _v) { x += _v.x; y += _v.y; z += _v.z; w += _v.w; return *this; }
Vec4& Vec4::operator-=(const Vec4& _v) { x -= _v.x; y -= _v.y; z -= _v.z; w -= _v.w; return *this; }
Vec4& Vec4::operator*=(const Vec4& _v) { x *= _v.x; y *= _v.y; z *= _v.z; w *= _v.w; return *this; }
Vec4& Vec4::operator*=(float _val) { x *= _val; y *= _val; z *= _val; w *= _val; return *this; }
Vec4& Vec4::operator/=(float _val) { x /= _val; y /= _val; z /= _val; w /= _val; return *this; }

float Vec4::LengthSq(void) const { return SQ(x) + SQ(y) + SQ(z) + SQ(w); }
float Vec4::Length(void) const { return sqrtf(LengthSq()); }

Vec4 Vec4::Normalize(void)
{
	float len = Length();
	return len > 0 ? *this / len : *this;
}

Vec4 Vec4::Project(void) { return w > 0 ? *this / w : *this; }

float Vec4::NormalizeGetLength(void)
{
	float len = Length();
	if (len > 0)
		*this /= len;
	return len;
}

float Vec4::Max() const { return max(x, max(y, max(w, z))); }

Vec4 Vec4::Max(const Vec4& v1, const Vec4& v2)
{
	return Vec4(max(v1.x, v2.x),
		max(v1.y, v2.y),
		max(v1.z, v2.z),
		max(v1.w, v2.w));
}

float Vec4::Dot(const Vec4& _v1, const Vec4& _v2) {
	return _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z + _v1.w * _v2.w;
}

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Matrix

//------------------------------------------------------------------------------------------------

Matrix::Matrix()
{
	memset(m, 0, 16 * sizeof(float));
	m[0] = m[5] = m[10] = m[15] = 1;
}
Matrix::Matrix(float* _other) { memcpy(&m, _other, 16 * sizeof(float)); }

Matrix::Matrix(float _m1, float _m2, float _m3, float _m4,
	float _m5, float _m6, float _m7, float _m8,
	float _m9, float _m10, float _m11, float _m12,
	float _m13, float _m14, float _m15, float _m16)
{
	m[0] = _m1, m[1] = _m2, m[2] = _m3, m[3] = _m4;
	m[4] = _m5, m[5] = _m6, m[6] = _m7, m[7] = _m8;
	m[8] = _m9, m[9] = _m10, m[10] = _m11, m[11] = _m12;
	m[12] = _m13, m[13] = _m14, m[14] = _m15, m[15] = _m16;
}

Matrix Matrix::Identity()
{
	Matrix mat;
	memset(mat.m, 0, 16 * sizeof(float));
	mat[0] = mat[5] = mat[10] = mat[15] = 1;
	return mat;
}

Matrix Matrix::Transpose() const
{
	Matrix mat;

	mat.m[0] = m[0], mat.m[1] = m[4], mat.m[2] = m[8], mat.m[3] = m[12];
	mat.m[4] = m[1], mat.m[5] = m[5], mat.m[6] = m[9], mat.m[7] = m[13];
	mat.m[8] = m[2], mat.m[9] = m[6], mat.m[10] = m[10], mat.m[11] = m[14];
	mat.m[12] = m[3], mat.m[13] = m[7], mat.m[14] = m[11], mat.m[15] = m[15];

	return mat;
}

Matrix Matrix::Translation(const Vec3 v)
{
	Matrix mat;

	mat.a[0][3] = v.x;
	mat.a[1][3] = v.y;
	mat.a[2][3] = v.z;

	return mat;
}

Matrix Matrix::Scaling(const Vec3 v)
{
	Matrix mat;

	mat.m[0] = v.x;
	mat.m[5] = v.y;
	mat.m[10] = v.z;

	return mat;
}

Matrix Matrix::RotateX(float angle)
{
	angle *= RAD;
	float sint = sin(angle), cost = cos(angle);
	Matrix mat;

	mat.m[5] = mat.m[10] = cost;
	mat.m[6] = -sint;
	mat.m[9] = sint;

	return mat;
}

Matrix Matrix::RotateY(float angle)
{
	angle *= RAD;
	float sint = sinf(angle), cost = cosf(angle);
	Matrix mat;

	mat.m[0] = mat.m[10] = cost;
	mat.m[2] = -sint;
	mat.m[8] = sint;

	return mat;
}

Matrix Matrix::RotateZ(float angle)
{
	angle *= RAD;
	float sint = sinf(angle), cost = cosf(angle);
	Matrix mat;

	mat.m[0] = mat.m[5] = cost;
	mat.m[1] = -sint;
	mat.m[4] = sint;

	return mat;
}

Matrix Matrix::Projection(float _fov, float _aspect, float _near, float _far)
{
	float dFov = DegToRad(_fov); // contains conversion from degree
	float tanHalfFov = tan(dFov / 2.f);

	Matrix _mat;

	_mat.m[0] = 1.0f / (_aspect * tanHalfFov); //  x scale
	_mat.m[5] = 1.0f / tanHalfFov; // y scale
	_mat.m[10] = -_far / (_far - _near); // z scale

	_mat.m[11] = -(_far * _near) / (_far - _near); // perspective division
	_mat.m[14] = -1.0f; // z axis perspective division
	_mat.m[15] = 0.0f;

	return _mat;
}

Matrix Matrix::LookAt(Vec3 _from, Vec3 _to)
{
	Vec3 forward = _to - _from;
	forward.y = 0;
	if (forward.LengthSq() < 0.0001f)
		return Matrix::Identity();
	forward = forward.Normalize();

	Vec3 worldUp = Vec3::up;
	if (fabs(Vec3::Dot(forward, worldUp)) > 0.99f)
		worldUp = Vec3(0, 0, 1);
	Vec3 right = Vec3::Cross(worldUp, forward).Normalize();
	Vec3 up = Vec3::Cross(forward, right);

	Matrix mat;

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

Matrix Matrix::View(Vec3 _pos, Vec3 _forward)
{
	_forward = _forward.Normalize();
	Vec3 right = Vec3::Cross(Vec3(0, 1, 0), _forward);
	Vec3 up = Vec3::Cross(_forward, right);

	Matrix mat;


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

Matrix Matrix::View(Vec3 _pos, Vec3 _forward, Vec3 _right, Vec3 _up)
{
	Matrix mat;

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

Matrix Matrix::View(Matrix _world)
{
	Vec3 right = Vec3(_world.m[0], _world.m[1], _world.m[2]);
	Vec3 up = Vec3(_world.m[4], _world.m[5], _world.m[6]);
	Vec3 forward = Vec3(_world.m[8], _world.m[9], _world.m[10]);
	Vec3 pos = Vec3(_world.m[3], _world.m[7], _world.m[11]);

	Matrix mat;

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

Matrix Matrix::World(Vec3 _pos, Vec3 _scale, Vec3 _forward, Vec3 _right, Vec3 _up)
{
	Matrix mat;

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

Matrix Matrix::World(Vec3 _pos, Vec3 _scale)
{
	return Translation(_pos) * Scaling(_scale);
}

Vec3 Matrix::MulPoint(const Vec3& v) const
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

Vec3 Matrix::MulVec(const Vec3& v)
{
	return Vec3(
		(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
		(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
		(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
}

Matrix Matrix::Mul(const Matrix& matrix) const
{
	Matrix ret;

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

Matrix Matrix::Inverse() const
{
	Matrix inv;
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
		// Handle this case
	}
	det = 1.0 / det;

	for (int i = 0; i < 16; i++)
		inv[i] = inv[i] * det;

	return inv;
}

float& Matrix::operator[](int _index) { return m[_index]; }
Matrix Matrix::operator*(const Matrix& _other) { return Mul(_other); }
Matrix Matrix::operator*=(const Matrix& _other) { *this = Mul(_other); return *this; }

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Color

//------------------------------------------------------------------------------------------------

Color::Color() { r = g = b = a = 1; }
Color::Color(float _r, float _g, float _b, float _a) { r = _r; g = _g; b = _b; a = _a; }

Color Color::operator+(const Color& _col)
{
	return Color(r + _col.r,
		g + _col.g,
		b + _col.b,
		a + _col.a);
}

Color Color::operator-(const Color& _col) { return Color(r - _col.r, g - _col.g, b - _col.b, a - _col.a); }
Color Color::operator*(const Color& _col) { return Color(r * _col.r, g * _col.g, b * _col.b, a * _col.a); }
Color Color::operator*(float _val) { return Color(r * _val, g * _val, b * _val, a * _val); }
Color Color::operator/(float _val) { return Color(r / _val, g / _val, b / _val, a / _val); }
Color& Color::operator+=(const Color& _col) { r += _col.r; g += _col.g; b += _col.b; a += _col.a; return *this; }
Color& Color::operator-=(const Color& _col) { r += r - _col.r; g += g - _col.g; b += b - _col.b; a += a - _col.a; return *this; }
Color& Color::operator*=(const Color& _col) { r *= _col.r; g *= _col.g; b *= _col.b; a *= _col.a; return *this; }
Color& Color::operator*=(float _val) { r *= _val; g *= _val; b *= _val; a *= _val; return *this; }
Color& Color::operator/=(float _val) { r /= _val; g /= _val; b /= _val; a /= _val; return *this; }

float Color::operator[](int index) { return v[index]; }

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Shading Frame

//------------------------------------------------------------------------------------------------

ShadingFrame::ShadingFrame(Vec3& _normal)
{
	normal = _normal.Normalize();

	Vec3 tmp = std::abs(normal.x) < 0.9f ? Vec3(1, 0, 0) : Vec3(0, 1, 0);
	v = Vec3::Cross(normal, tmp).Normalize();
	u = Vec3::Cross(normal, v).Normalize();
}

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Quaternion

//------------------------------------------------------------------------------------------------

float Quaternion::LengthSq() const { return SQ(w) + SQ(x) + SQ(y) + SQ(z); }
float Quaternion::Length() const { return sqrt(LengthSq()); }

float Quaternion::Dot(Quaternion& _q1, Quaternion& _q2) { return _q1.w * _q2.w + _q1.x * _q2.x + _q1.y * _q2.y + _q1.z + _q2.z; }

Quaternion Quaternion::Slerp(Quaternion _q1, Quaternion _q2, float _time)
{
	float dot = Dot(_q1, _q2);

	// Handle cases where quaternions are nearly opposite
	if (dot < 0.0f) {
		_q1 = -_q1;
		dot = -dot;
	}

	// Handle near-parallel quaternions to avoid division by zero
	const float tolerance = 1e-6f;
	if (dot > 1.0f - tolerance) {
		return Lerp(_q1, _q2, _time);
	}

	float theta = acos(dot);
	float sinThetaInv = 1.0f / sin(theta);
	float sinTheta1 = sin((1.0f - _time) * theta) * sinThetaInv;
	float sinTheta2 = sin(_time * theta) * sinThetaInv;

	return _q1 * sinTheta1 + _q2 * sinTheta2;
}

Quaternion Quaternion::Normalize()
{
	float len = Length();
	return len > 0 ? *this / len : *this;
}

Vec3 Quaternion::ToVector() { return Vec3(x, y, z); }

Matrix Quaternion::ToMatrix() const
{
	float xx = q[0] * q[0], xy = q[0] * q[1];
	float xz = q[0] * q[2], yy = q[1] * q[1];
	float zz = q[2] * q[2], yz = q[1] * q[2];
	float wx = q[3] * q[0], wy = q[3] * q[1];
	float wz = q[3] * q[2];

	Matrix mat;

	mat[0] = 1.0f - 2.0f * (yy + zz);
	mat[1] = 2.0f * (xy - wz);
	mat[2] = 2.0f * (xz + wy);

	mat[4] = 2.0f * (xy + wz);
	mat[5] = 1.0f - 2.0f * (xx + zz);
	mat[6] = 2.0f * (yz - wx);

	mat[8] = 2.0f * (xz - wy);
	mat[9] = 2.0f * (yz + wx);
	mat[10] = 1.0f - 2.0f * (xx + yy);

	return mat;
}

Quaternion Quaternion::Conjugate() { return Quaternion(w, -x, -y, -z); }
Quaternion Quaternion::Inverse() { return Conjugate() / Length(); }

Quaternion Quaternion::FromAxisAngle(const Vec3& _axis, float _angle) {
	float hAngle = DegToRad(_angle) * 0.5f;
	float sinH = sinf(hAngle);
	float cosH = cosf(hAngle);

	return Quaternion(cosH, sinH * _axis.x, sinH * _axis.y, sinH * _axis.z);
}

Quaternion Quaternion::FromVector(Vec3 _vec)
{
	return Quaternion(0, _vec.x, _vec.y, _vec.z).Normalize();
}

Quaternion Quaternion::FromEuler(Vec3 _angle)
{
	_angle *= RAD;
	float cx = cos(_angle.x / 2.f);
	float sx = sin(_angle.x / 2.f);

	float cy = cos(_angle.y / 2.f);
	float sy = sin(_angle.y / 2.f);

	float cz = cos(_angle.z / 2.f);
	float sz = sin(_angle.z / 2.f);

	Quaternion q;
	q.w = cx * cy * cz + sx * sy * sz;
	q.x = sx * cy * cz - cx * sy * sz;
	q.y = cx * sy * cz + sx * cy * sz;
	q.z = cx * cy * sz - sx * sy * cz;

	return q;
}

Vec3 Quaternion::RotateVector(Vec3 _v)
{
	Quaternion rot = Normalize();
	Quaternion point(0, _v.x, _v.y, _v.z);
	Quaternion res = rot * point * rot.Conjugate();
	return Vec3(res.x, res.y, res.z);
}

Quaternion Quaternion::operator+(const Quaternion& _other) { return Quaternion(w + _other.w, x + _other.x, y + _other.y, z + _other.z); }
Quaternion Quaternion::operator-(const Quaternion& _other) { return Quaternion(w - _other.w, x - _other.x, y - _other.y, z - _other.z); }
Quaternion Quaternion::operator*(float _val) { return Quaternion(w * _val, x * _val, y * _val, z * _val); }
Quaternion Quaternion::operator/(float _val) { return Quaternion(w / _val, x / _val, y / _val, z / _val); }
Quaternion Quaternion::operator-() { return Quaternion(-w, -x, -y, -z); }

Quaternion& Quaternion::operator+=(const Quaternion& _other) { *this = *this + _other; return *this; }
Quaternion& Quaternion::operator-=(const Quaternion& _other) { *this = *this - _other; return *this; }
Quaternion& Quaternion::operator*=(float _val) { w *= _val; x *= _val; y *= _val; z *= _val; return *this; }
Quaternion& Quaternion::operator/=(float _val) { w /= _val; x /= _val; y /= _val; z /= _val; return *this; }

Quaternion Quaternion::operator*(const Quaternion& _qt)
{
	return Quaternion(
		w * _qt.w - x * _qt.x - y * _qt.y - z * _qt.z,
		w * _qt.x + x * _qt.w + y * _qt.z - z * _qt.y,
		w * _qt.y - x * _qt.z + y * _qt.w + z * _qt.x,
		w * _qt.z + x * _qt.y - y * _qt.x + z * _qt.w
	);
}

Quaternion Quaternion::operator*=(const Quaternion& _qt)
{
	w = w * _qt.w - x * _qt.x - y * _qt.y - z * _qt.z;
	x = w * _qt.x + x * _qt.w + y * _qt.z - z * _qt.y;
	y = w * _qt.y - x * _qt.z + y * _qt.w + z * _qt.x;
	z = w * _qt.z + x * _qt.y - y * _qt.x + z * _qt.w;
	return *this;
}

Vec3 Quaternion::operator*(const Vec3& _v) { return RotateVector(_v); }

//------------------------------------------------------------------------------------------------

#pragma endregion

#pragma region Spherical Coordinates

//------------------------------------------------------------------------------------------------

SphericalCoordinate::SphericalCoordinate()
{
	r = 1;
	theta = 0;
	phi = 0;
}

SphericalCoordinate::SphericalCoordinate(float _radius, float _theta, float _phi)
{
	r = _radius > 0 ? _radius : 1;
	theta = _theta;
	phi = _phi;

	Normalize();
}

void SphericalCoordinate::Normalize()
{
	// Clamp theta to [0, π]
	theta = clamp(theta, 0, PI);

	// Wrap phi to [0, 2π]
	phi = fmod(phi, PI2);
	if (phi < 0) phi += PI2;
}

Vec3 SphericalCoordinate::ToCartesian() const {
	return Vec3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi)) * r;
}

SphericalCoordinate SphericalCoordinate::fromCartesian(Vec3 _v) {
	float r = _v.Length();

	// if length in 0 return
	if (r == 0)
		return SphericalCoordinate();

	float theta = acos(_v.y / r); // vertical rotation (in radians)
	float phi = atan2(_v.x, _v.z); // horizontal rotation (in radians)
	return SphericalCoordinate(r, theta, phi);
}

void SphericalCoordinate::Rotate(float _horiz, float _vert)
{
	phi += DegToRad(_horiz);
	theta += DegToRad(_vert);
	Normalize();
}

//------------------------------------------------------------------------------------------------

#pragma endregion


#pragma region ostream operator

//------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, Vec2 _v) {
	return os << "(" << _v.x << "," << _v.y << ")";
}

std::ostream& operator<<(std::ostream& os, Vec3 _v) {
	return os << "(" << _v.x << "," << _v.y << "," << _v.z << ")";
}

std::ostream& operator<<(std::ostream& os, Vec4 _v) {
	return os << "(" << _v.x << "," << _v.y << "," << _v.z << "," << _v.w << ")";
}

std::ostream& operator<<(std::ostream& os, Matrix _mat)
{
	os << "[" << _mat[0] << "," << _mat[1] << "," << _mat[2] << "," << _mat[3] << "]\n";
	os << "[" << _mat[4] << "," << _mat[5] << "," << _mat[6] << "," << _mat[7] << "]\n";
	os << "[" << _mat[8] << "," << _mat[9] << "," << _mat[10] << "," << _mat[11] << "]\n";
	os << "[" << _mat[12] << "," << _mat[13] << "," << _mat[14] << "," << _mat[15] << "]\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, Color& _color)
{
	os << "(" << static_cast<int>(_color.r) << ","
		<< static_cast<int>(_color.g) << ","
		<< static_cast<int>(_color.b) << ","
		<< static_cast<int>(_color.a) << ")\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, ShadingFrame& _sf)
{
	std::cout << _sf.u << "]\n";
	std::cout << _sf.v << "]\n";
	std::cout << _sf.normal << "]\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, Quaternion& _quat)
{
	os << "(" << _quat.w << "," << _quat.x << "," << _quat.y << "," << _quat.z << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, SphericalCoordinate& _sc) {
	return os << "( " << _sc.r << "," << _sc.theta << "," << _sc.phi << " )";
}

//------------------------------------------------------------------------------------------------

#pragma endregion
