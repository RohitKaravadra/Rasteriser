#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <ostream>

#define SQ(x) (x * x)
#define min(a,b)(a>b?b:a)
#define max(a,b)(a<b?b:a)
#define clamp(_val,_min,_max) (max(_min, min(_max,_val)))

template<typename T>
static T lerp(const T a, const T b, float t) { return a * (1.0f - t) + (b * t); }

class Vec2
{
public:
	union {
		float v[2];
		struct { float x, y; };
	};

	Vec2() { x = y = 0; }
	Vec2(float _x, float _y) { x = _x; y = _y; }

	Vec2 operator+(const Vec2& _v) const;
	Vec2 operator-(const Vec2& _v) const;
	Vec2 operator*(const Vec2& _v) const;
	Vec2 operator*(const float _val) const;
	Vec2 operator/(const float _val) const;

	Vec2& operator+=(const Vec2& _v);
	Vec2& operator-=(const Vec2& _v);
	Vec2& operator*=(const Vec2& _v);
	Vec2& operator*=(const float _val);
	Vec2& operator/=(const float _val);

	Vec2 operator-() const;
	friend std::ostream& operator<<(std::ostream& os, Vec2 _v);

	float LengthSq(void) const;
	float Lenght(void) const;

	Vec2 Normalize(void);
	float NormalizeGetLength(void);

	float Max() const;
	float Min() const;
	static Vec2 Max(const Vec2& _v1, const Vec2& _v2);
	static Vec2 Min(const Vec2& _v1, const Vec2& _v2);

	static float Dot(const Vec2& _v1, const Vec2& _v2);
	static float Cross(const Vec2& _v1, const Vec2& _v2);
};

class Vec3
{
public:
	union {
		float v[3];
		struct { float x, y, z; };
	};

	Vec3() { x = y = z = 0; }
	Vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

	Vec3 operator+(const Vec3& _v) const;
	Vec3 operator-(const Vec3& _v) const;
	Vec3 operator*(const Vec3& _v) const;
	Vec3 operator*(const float _val) const;
	Vec3 operator/(const float _val) const;
	Vec3 operator-() const;

	Vec3& operator+=(const Vec3& _v);
	Vec3& operator-=(const Vec3& _v);
	Vec3& operator*=(const Vec3& _v);
	Vec3& operator*=(const float _val);
	Vec3& operator/=(const float _val);

	friend std::ostream& operator<<(std::ostream& os, Vec3 _v);

	float LengthSq(void) const;
	float Length(void) const;

	Vec3 Normalize(void);
	float NormalizeGetLength(void);

	static float Dot(const Vec3& _v1, const Vec3& _v2);
	static Vec3 Cross(const Vec3& _v1, const Vec3& _v2);

	float Max(void) const;
	float Min(void) const;

	Vec3 Max(const Vec3& v1, const Vec3& v2);
	Vec3 Min(const Vec3& v1, const Vec3& v2);
};

class Vec4
{
public:
	union {
		float v[4];
		struct { float x, y, z, w; };
	};

	Vec4() { x = y = z = w = 0; }
	Vec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }

	Vec4 operator+(const Vec4& _v) const;
	Vec4 operator-(const Vec4& _v) const;
	Vec4 operator*(const Vec4& _v) const;
	Vec4 operator*(const float _val) const;
	Vec4 operator/(const float _val) const;

	Vec4& operator+=(const Vec4& _v);
	Vec4& operator-=(const Vec4& _v);
	Vec4& operator*=(const Vec4& _v);
	Vec4& operator*=(const float _val);
	Vec4& operator/=(const float _val);

	Vec4 operator-() const;
	friend std::ostream& operator<<(std::ostream& os, Vec4 _v);

	float LengthSq(void) const;
	float Length(void) const;

	Vec4 Normalize(void);
	float NormalizeGetLength(void);
	Vec4 Project(void);

	float Max() const;
	static Vec4 Max(const Vec4& v1, const Vec4& v2);
	static float Dot(const Vec4& _v1, const Vec4& _v2);
};

class Matrix
{
public:
	union {
		float a[4][4];
		float m[16];
	};

	Matrix();
	Matrix(float* _other);
	Matrix(float _m1, float _m2, float _m3, float _m4,
		float _m5, float _m6, float _m7, float _m8,
		float _m9, float _m10, float _m11, float _m12,
		float _m13, float _m14, float _m15, float _m16);

	void Identity();

	Matrix Transpose() const;

	static Matrix Translation(const Vec3 v);
	static Matrix Scaling(const Vec3 v);
	static Matrix RotateX(float angle);
	static Matrix RotateY(float angle);
	static Matrix RotateZ(float angle);

	static Matrix Projection(float _fov, float _aspect, float _near, float _far);
	static Matrix LookAt(Vec3 _from, Vec3 _to, Vec3 _up);
	static Matrix View(Vec3 _pos, Vec3 _forward);

	Vec3 MulPoint(const Vec3& v) const;
	Vec3 MulVec(const Vec3& v);

	Matrix Mul(const Matrix& matrix) const;
	Matrix Inverse() const;

	Matrix operator*(Matrix& _other);
	float& operator[](int _index);
	friend std::ostream& operator<<(std::ostream& os, Matrix _mat);
};

class Color
{
public:
	union {
		float v[4];
		struct { float r, g, b, a; };

	};

	Color();
	Color(float _r, float _g, float _b, float _a = 1);

	Color operator+(const Color& _col);
	Color operator-(const Color& _col);
	Color operator*(const Color& _col);
	Color operator*(const float _val);
	Color operator/(const float _val);

	Color& operator+=(const Color& _col);
	Color& operator-=(const Color& _col);
	Color& operator*=(const Color& _col);
	Color& operator*=(const float _val);
	Color& operator/=(const float _val);

	float operator[](int index);
	friend std::ostream& operator<<(std::ostream& os, Color& _color);
};

class ShadingFrame
{
public:
	union {
		float m[9];
		float a[3][3];
	};

	ShadingFrame(Vec3& _normal);
	friend std::ostream& operator<<(std::ostream& os, ShadingFrame& _sf);
};

class Quaternion
{
public:
	union {
		float v[4];
		struct { float a, b, c, d; };
	};

	Quaternion() { a = b = c = d = 1; };
	Quaternion(float _a, float _b, float _c, float _d) { a = _a; b = _b; c = _c; d = _d; };

	float LengthSq() const;
	float Length() const;

	static float Dot(Quaternion& _q1, Quaternion& _q2);
	static Quaternion Slerp(Quaternion& _q1, Quaternion& _q2, float _time);

	Quaternion Normalize();
	Quaternion Conjugate();
	Quaternion Inverse();

	Matrix ToMatrix() const;
	Vec3 RotatePoint(Vec3 _v);

	Quaternion operator+(const Quaternion& _other);
	Quaternion operator-(const Quaternion& _other);
	Quaternion operator*(const float _val);
	Quaternion operator/(const float _val);

	Quaternion& operator+=(const Quaternion& _other);
	Quaternion& operator-=(const Quaternion& _other);
	Quaternion& operator*=(const float _val);
	Quaternion& operator/=(const float _val);

	Quaternion operator*(const Quaternion& _qt);
	Quaternion operator*=(const Quaternion& _qt);

	Vec3 operator*(const Vec3& _v);
	friend std::ostream& operator<<(std::ostream& os, Quaternion& _quat);
};