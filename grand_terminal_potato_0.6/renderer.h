#ifndef RENDERER_H
#define RENDERER_H

#include "matrix4x4.h"
#include "mesh.h"

#include <vector>

class Renderer
{
public:
	std::vector<Triangle> worldBuffer;
	std::vector<Triangle4D> clipBuffer;
	std::vector<Triangle> screenBuffer;

	Renderer() = default;

	void clearBuffers();
	void addMeshToWorld(const Mesh& mesh, const Matrix4x4& worldMat, const Vector3& lightDir);
	void perspectiveProjection(const Matrix4x4& projectionMat, const Matrix4x4& lookAtMat, const Vector3& cameraPos);

private:
	void clipping();
	void sortingZ();
	Vector4 linePlaneIntersection(const Vector4& normal, const Vector4& p1, const Vector4& p2);
	int clipAgainstPlane(Vector4 plane, const Triangle4D& in_tri, Triangle4D& out_tri1, Triangle4D& out_tri2);
};

#endif
