#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh.h"
#include "renderer.h"

#include <vector>
#include <queue>
#include <algorithm>

char getShadeChar(float intensity)
{
	char ramp[] = ".:-=+*#%@";
	int rampLen{ (sizeof(ramp) / sizeof(char)) - 1 };

	if (std::isnan(intensity)) intensity = 0.0f; // guard against NaN
	intensity = (intensity < 0.0f) ? 0.0f : (intensity > 1.0f ? 1.0f : intensity);

	int index{ static_cast<int>((rampLen - 1) * intensity) };
	return ramp[index];
}

void Renderer::clearBuffers()
{
	worldBuffer.clear();
	clipBuffer.clear();
	screenBuffer.clear();
}

void Renderer::addMeshToWorld(const Mesh& mesh, const Matrix4x4& worldMat, const Vector3& lightDir)
{
	for (const Triangle& triLocal : mesh.triangles)
	{
		Triangle triWorld
		{
			worldMat * triLocal.vertices[0],
			worldMat * triLocal.vertices[1],
			worldMat * triLocal.vertices[2]
		};

		triWorld.symbol = getShadeChar(Vector3::dot(triWorld.getNormal(), lightDir));

		worldBuffer.push_back(triWorld);
	}
}

void Renderer::perspectiveProjection(const Matrix4x4& projectionMat, const Matrix4x4& lookAtMat, const Vector3& cameraPos)
{
	for (const Triangle& triWorld : worldBuffer)
	{
		Vector3 dirToCamera{ (cameraPos - triWorld.vertices[0]).normalized() };
		if (Vector3::dot(triWorld.getNormal(), dirToCamera) > 0) continue;

		Triangle triLookAt
		{
			lookAtMat * triWorld.vertices[0],
			lookAtMat * triWorld.vertices[1],
			lookAtMat * triWorld.vertices[2]
		};

		Triangle4D triProjected
		{
			projectionMat * triLookAt.vertices[0].toPoint4(),
			projectionMat * triLookAt.vertices[1].toPoint4(),
			projectionMat * triLookAt.vertices[2].toPoint4(),
		};

		triProjected.symbol = triWorld.symbol;

		clipBuffer.push_back(triProjected);
	}

	clipping();
}

void Renderer::clipping()
{
	Vector4 planes[6] =
	{
		{ 1,  0,  0,  1}, // 0 : left
		{-1,  0,  0,  1}, // 1 : right
		{ 0, -1,  0,  1}, // 2 : top
		{ 0,  1,  0,  1}, // 3 : bottom
		{ 0,  0,  1,  1}, // 4 : far
		{ 0,  0, -1,  1}, // 5 : near
	};

	for (const Triangle4D& triToClip : clipBuffer)
	{
		std::vector<Triangle4D> clipQueue;
		clipQueue.push_back(triToClip);

		for (int p = 0; p < 6; ++p)
		{
			std::vector<Triangle4D> newQueue;
			for (const Triangle4D& tri : clipQueue)
			{
				Triangle4D clippedTris[2];
				int triCount = clipAgainstPlane(planes[p], tri, clippedTris[0], clippedTris[1]);

				for (int i = 0; i < triCount; ++i)
				{
					clippedTris[i].symbol = triToClip.symbol;
					newQueue.push_back(clippedTris[i]);
				}
			}

			clipQueue = newQueue;

			if (clipQueue.empty()) break;
		}

		for (const Triangle4D& survivingTri : clipQueue)
		{
			Triangle perspectiveDividedTri;
			for (int i = 0; i < 3; ++i)
			{
				float w = survivingTri.vertices[i].w;
				perspectiveDividedTri.vertices[i] =
				{
					survivingTri.vertices[i].x / w,
					survivingTri.vertices[i].y / w,
					survivingTri.vertices[i].z / w
				};
			}

			perspectiveDividedTri.symbol = survivingTri.symbol;

			screenBuffer.push_back(perspectiveDividedTri);
		}
	}

	sortingZ();
}

void Renderer::sortingZ()
{
	std::sort(screenBuffer.begin(), screenBuffer.end(), [](const Triangle& tri1, const Triangle& tri2)
	{
		return tri1.getAverageZ() > tri2.getAverageZ();
	});
}

// THIS 4D DOT PRODUCT IS FASCINATING. SPENT SOME MORE TIME ON IT TO FULLY UNDERSTAND
Vector4 Renderer::linePlaneIntersection(const Vector4& n, const Vector4& p1, const Vector4& p2)
{
	float d1{ Vector4::dot(p1, n) };
	float d2{ Vector4::dot(p2, n) };
	float t{ d1 / (d1 - d2) };

	return 
	{ 
		p1.x + t * (p2.x - p1.x),
		p1.y + t * (p2.y - p1.y),
		p1.z + t * (p2.z - p1.z),
		p1.w + t * (p2.w - p1.w)
	};
}

int Renderer::clipAgainstPlane(Vector4 plane, const Triangle4D& in_tri, Triangle4D& out_tri1, Triangle4D& out_tri2)
{
	const Vector4* inPoints[3];  int inCount = 0;
	const Vector4* outPoints[3]; int outCount = 0;

	for (int i = 0; i < 3; ++i)
	{
		bool isInside = Vector4::dot(in_tri.vertices[i], plane) >= 0.0f;

		if (isInside) inPoints[inCount++] = &in_tri.vertices[i];
		else outPoints[outCount++] = &in_tri.vertices[i];
	}

	if (inCount == 0) return 0;

	if (inCount == 3)
	{
		out_tri1 = in_tri;
		return 1;
	}

	if (inCount == 1)
	{
		out_tri1.vertices[0] = *inPoints[0];
		out_tri1.vertices[1] = linePlaneIntersection(plane, *inPoints[0], *outPoints[0]);
		out_tri1.vertices[2] = linePlaneIntersection(plane, *inPoints[0], *outPoints[1]);

		return 1;
	}

	if (inCount == 2)
	{
		out_tri1.vertices[0] = *inPoints[0];
		out_tri1.vertices[1] = *inPoints[1];
		out_tri1.vertices[2] = linePlaneIntersection(plane, *inPoints[0], *outPoints[0]);
		
		out_tri2.vertices[0] = *inPoints[1];
		out_tri2.vertices[1] = out_tri1.vertices[2];
		out_tri2.vertices[2] = linePlaneIntersection(plane, *inPoints[1], *outPoints[0]);

		return 2;
	}

	return 0;
}