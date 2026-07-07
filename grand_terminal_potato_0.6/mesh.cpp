#include "vector3.h"
#include "mesh.h"

#include <string>
#include <string_view>
#include <fstream>
#include <sstream>

#include <vector>
#include <initializer_list>

// Triangle Class

Triangle::Triangle(const Vector3& set_v0, const Vector3& set_v1, const Vector3& set_v2)
	: vertices{ set_v0, set_v1, set_v2 }
{
}

Triangle::Triangle(const Vector3& set_v0, const Vector3& set_v1, const Vector3& set_v2, char set_symbol)
	:	vertices {set_v0, set_v1, set_v2}, symbol {set_symbol}
{
}

Vector3 Triangle::getNormal() const
{
	Vector3 edge0{ vertices[1] - vertices[0] };
	Vector3 edge1{ vertices[2] - vertices[0] };
	return Vector3::cross(edge1, edge0).normalized();
}

float Triangle::getAverageZ() const
{
	return (vertices[0].z + vertices[1].z + vertices[2].z) / 3.0f;
}

// Mesh Class

bool Mesh::loadFromObjectFile(const std::string& filename)
{
    std::ifstream objFile(filename);

    if (objFile.fail()) return false;

    std::vector<Vector3> vertexArr;
    std::string line;

    while (std::getline(objFile, line))
    {
        if (line.empty()) continue;

        std::stringstream sstream(line);

        char junk;

        if (line[0] == 'v')
        {
            Vector3 vertex;
            sstream >> junk >> vertex.x >> vertex.y >> vertex.z;
            vertexArr.push_back(vertex);
        }

        if (line[0] == 'f')
        {
            int face[3];
            sstream >> junk >> face[0] >> face[1] >> face[2];
            triangles.push_back({ vertexArr[face[0] - 1], vertexArr[face[1] - 1], vertexArr[face[2] - 1] });
        }
    }

    return true;
}