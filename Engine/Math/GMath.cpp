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

Vec3 Vec4::ToVec3(void) { return Vec3(x, y, z); }

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
	Vec3 tmp = std::abs(normal.y) < 0.99f ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
	tangent = Vec3::Cross(normal, tmp);
	binormal = Vec3::Cross(tangent, normal);
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
	std::cout << _sf.binormal << "]\n";
	std::cout << _sf.tangent << "]\n";
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
