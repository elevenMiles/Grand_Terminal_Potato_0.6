#ifndef MESH_H
#define MESH_H

#include "vector3.h"
#include "vector4.h"

#include <vector>
#include <initializer_list>

class Triangle
{
public:
	Vector3 vertices[3]; // Should be defined in clockwise order
	char symbol{};

	Triangle() = default;
	Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);
	Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, char symbol);

	Vector3 getNormal() const;
	float getAverageZ() const;
};

struct Triangle4D
{
	Vector4 vertices[3]; // Should be defined in clockwise order
	char symbol{};
};

class Mesh
{
public:
	std::vector<Triangle> triangles;

	bool loadFromObjectFile(const std::string& filename);
};

#endif
