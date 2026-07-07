#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "vector3.h"
#include "vector4.h"

#include <iostream>

class Matrix4x4
{
private:
	Vector4 cols[4];

public:
	Matrix4x4() = default;
	Matrix4x4(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3);

	const Vector4& getCol(int c) const;

	static Matrix4x4 identity();

	static Matrix4x4 rotationX(float angleDeg);
	static Matrix4x4 rotationY(float angleDeg);
	static Matrix4x4 rotationZ(float angleDeg);
	static Matrix4x4 translation(const Vector3& p);
	static Matrix4x4 scale(float factor);

	static Matrix4x4 projection(float fov, float near, float far, float aspectRatio);
	static Matrix4x4 pointAt(const Vector3& pos, const Vector3& target, const Vector3& up);
	
	Matrix4x4 quickInverse() const;

	friend Vector3 operator*(const Matrix4x4& m, const Vector3& v);
	friend Vector4 operator*(const Matrix4x4& m, const Vector4& v);
	friend Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

	friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& m);
};

#endif
