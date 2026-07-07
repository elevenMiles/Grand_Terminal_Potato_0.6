#ifndef VECTOR4_H
#define VECTOR4_H

#include <iostream>

class Vector4
{
public:
	float x{};
	float y{};
	float z{};
	float w{};

	Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

	static float dot(const Vector4& v1, const Vector4& v2);
	friend std::ostream& operator<<(std::ostream& os, const Vector4& v);
};

#endif