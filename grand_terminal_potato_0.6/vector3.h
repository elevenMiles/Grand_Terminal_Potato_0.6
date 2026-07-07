#ifndef VECTOR3_H
#define VECTOR3_H

#include "vector4.h"

#include <iostream>

class Vector4;

class Vector3
{
public:	
	float x{};
	float y{};
	float z{};

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	Vector3(const Vector4& v4);

	Vector3 normalized() const;
	Vector4 toPoint4() const;
	Vector4 toDirection4() const;

	static float dot(const Vector3& v1, const Vector3& v2);
	static Vector3 cross(const Vector3& v1, const Vector3& v2);

	friend Vector3 operator-(const Vector3& v);
	friend Vector3 operator-(const Vector3& v1, const Vector3& v2);
	friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
	friend Vector3 operator*(const Vector3& v, float s);

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

#endif