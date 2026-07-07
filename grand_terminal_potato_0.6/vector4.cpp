#include "vector4.h"

#include <iostream>

Vector4::Vector4(float set_x, float set_y, float set_z, float set_w)
	: x {set_x}, y {set_y}, z {set_z}, w {set_w}
{
}

float Vector4::dot(const Vector4& v1, const Vector4& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
	return os;
}