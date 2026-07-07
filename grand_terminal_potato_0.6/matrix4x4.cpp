#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"

#include <cmath>
#include <iostream>

constexpr float DEG_TO_RAD{ 3.14159265359f / 180.0f };

Matrix4x4::Matrix4x4(const Vector4& set_col0, const Vector4& set_col1, const Vector4& set_col2, const Vector4& set_col3)
	: cols {set_col0, set_col1, set_col2, set_col3}
{
}

const Vector4& Matrix4x4::getCol(int c) const
{
	if (c >= 0 || c < 4) return cols[c];
	return Vector4{};
}

Matrix4x4 Matrix4x4::identity()
{
	return
	{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
}

Matrix4x4 Matrix4x4::rotationX(float angleDeg)
{
	float cosX{std::cos(angleDeg * DEG_TO_RAD)};
	float sinX{std::sin(angleDeg * DEG_TO_RAD)};

	return
	{
		{1.0f,  0.0f,   0.0f,  0.0f},
		{0.0f,  cosX, -sinX, 0.0f},
		{0.0f,  sinX,  cosX, 0.0f},
		{0.0f,  0.0f,   0.0f,  1.0f}
	};
}

Matrix4x4 Matrix4x4::rotationY(float angleDeg)
{
	float cosY{ std::cos(angleDeg * DEG_TO_RAD) };
	float sinY{ std::sin(angleDeg * DEG_TO_RAD) };

	return
	{
		{ cosY,  0.0f,  sinY, 0.0f},
		{ 0.0f,   1.0f,  0.0f,  0.0f},
		{-sinY,  0.0f,  cosY, 0.0f},
		{ 0.0f,   0.0f,  0.0f,  1.0f}
	};
}

Matrix4x4 Matrix4x4::rotationZ(float angleDeg)
{
	float cosZ{ std::cos(angleDeg * DEG_TO_RAD) };
	float sinZ{ std::sin(angleDeg * DEG_TO_RAD) };

	return
	{
		{cosZ, -sinZ,  0.0f,  0.0f},
		{sinZ,  cosZ,  0.0f,  0.0f},
		{0.0f,   0.0f,   1.0f,  0.0f},
		{0.0f,   0.0f,   0.0f,  1.0f}
	};
}

Matrix4x4 Matrix4x4::translation(const Vector3& p)
{
	return
	{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		p.toPoint4()
	};
}

Matrix4x4 Matrix4x4::projection(float fov, float near, float far, float aspectRatio)
{
	float scale{ 1.0f / std::tan(fov * 0.5f * DEG_TO_RAD)};
	float a{ -far / (far - near) };
	float b{ -(far * near) / (far - near) };

	return
	{
		{scale / aspectRatio, 0.0f,   0.0f,  0.0f},
		{0.0f,				  scale, 0.0f,  0.0f},
		{0.0f,				  0.0f,   a,   -1.0f},
		{0.0f,				  0.0f,   b,    0.0f}
	};
}

Matrix4x4 Matrix4x4::scale(float s)
{
	return
	{
		{s,   0.0f, 0.0f, 0.0f},
		{0.0f, s,   0.0f, 0.0f},
		{0.0f, 0.0f, s,   0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
}

Matrix4x4 Matrix4x4::pointAt(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	// calculate forward
	Vector3 newForward{ (pos - target).normalized() };

	// calculate up
	Vector3 a{ newForward * Vector3::dot(up, newForward) };
	Vector3 newUp{ (up - a).normalized() };

	// calculate right
	Vector3 newRight{ Vector3::cross(newUp, newForward) };


	return { newRight.toDirection4(), newUp.toDirection4(), newForward.toDirection4(), pos.toPoint4() };
}

Matrix4x4 Matrix4x4::quickInverse() const
{
	Matrix4x4 inverseMat
	{
		{cols[0].x, cols[1].x, cols[2].x, 0.0f},
		{cols[0].y, cols[1].y, cols[2].y, 0.0f},
		{cols[0].z, cols[1].z, cols[2].z, 0.0f},
		{      0.0f,       0.0f,       0.0f, 1.0f}
	};

	Vector4 newPos{ inverseMat * cols[3] };
	inverseMat.cols[3] = { -newPos.x, -newPos.y, -newPos.z, 1.0f };

	return inverseMat;
}

Vector3 operator*(const Matrix4x4& m, const Vector3& v)
{
	return m * v.toPoint4();
}

Vector4 operator*(const Matrix4x4& m, const Vector4& v)
{
	return
	{
		m.cols[0].x * v.x + m.cols[1].x * v.y + m.cols[2].x * v.z + m.cols[3].x * v.w,
		m.cols[0].y * v.x + m.cols[1].y * v.y + m.cols[2].y * v.z + m.cols[3].y * v.w,
		m.cols[0].z * v.x + m.cols[1].z * v.y + m.cols[2].z * v.z + m.cols[3].z * v.w,
		m.cols[0].w * v.x + m.cols[1].w * v.y + m.cols[2].w * v.z + m.cols[3].w * v.w,
	};
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	for (int i{ 0 }; i < 4; ++i)
	{
		result.cols[i].x = m1.cols[0].x * m2.cols[i].x + m1.cols[1].x * m2.cols[i].y + m1.cols[2].x * m2.cols[i].z + m1.cols[3].x * m2.cols[i].w;
		result.cols[i].y = m1.cols[0].y * m2.cols[i].x + m1.cols[1].y * m2.cols[i].y + m1.cols[2].y * m2.cols[i].z + m1.cols[3].y * m2.cols[i].w;
		result.cols[i].z = m1.cols[0].z * m2.cols[i].x + m1.cols[1].z * m2.cols[i].y + m1.cols[2].z * m2.cols[i].z + m1.cols[3].z * m2.cols[i].w;
		result.cols[i].w = m1.cols[0].w * m2.cols[i].x + m1.cols[1].w * m2.cols[i].y + m1.cols[2].w * m2.cols[i].z + m1.cols[3].w * m2.cols[i].w;
	}

	return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix4x4& m)
{
	os << "[ " << m.cols[0].x << ' ' << m.cols[1].x << ' ' << m.cols[2].x << ' ' << m.cols[3].x << " ]\n";
	os << "[ " << m.cols[0].y << ' ' << m.cols[1].y << ' ' << m.cols[2].y << ' ' << m.cols[3].y << " ]\n";
	os << "[ " << m.cols[0].z << ' ' << m.cols[1].z << ' ' << m.cols[2].z << ' ' << m.cols[3].z << " ]\n";
	os << "[ " << m.cols[0].w << ' ' << m.cols[1].w << ' ' << m.cols[2].w << ' ' << m.cols[3].w << " ]\n";

	return os;
}