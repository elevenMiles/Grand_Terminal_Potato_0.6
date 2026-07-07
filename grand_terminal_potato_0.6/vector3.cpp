#include "vector3.h"
#include "vector4.h"

#include <cmath>
#include <iostream>

Vector3::Vector3(float set_x, float set_y, float set_z)
	: x {set_x}, y {set_y}, z {set_z}
{
}

Vector3::Vector3(const Vector4& v4)
	: x {v4.x}, y {v4.y}, z {v4.z} 
{
}

Vector3 Vector3::normalized() const
{
	float len{ std::sqrt(x * x + y * y + z * z) };
	if (len < 1e-12f) return { 0.0f, 0.0f, 0.0f };
	return { x / len, y / len, z / len };
}

Vector4 Vector3::toPoint4() const
{
	return { x, y, z, 1.0f };
}

Vector4 Vector3::toDirection4() const
{
	return { x, y, z, 0.0f };
}

float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2)
{
	return
	{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

Vector3 operator-(const Vector3& v)
{
	return {-v.x, -v.y, -v.z};
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 operator*(const Vector3& v, float s)
{
	return {v.x * s, v.y * s, v.z * s};
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
	return os;
}