#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"

#include "mesh.h"
#include "consoleScreen.h"

#include "renderer.h"

#include <chrono>
#include <Windows.h>
#include <vector>
#include <iostream>

int main()
{
	ConsoleScreen<256, 144> screen;
	Renderer renderer;

	Matrix4x4 projectionMat{ Matrix4x4::projection(45.0f, 0.1f, 1000.0f, screen.aspect_ratio()) };

	Mesh potato_city;
	potato_city.loadFromObjectFile("Objects\\potato_city.obj");

	Mesh player;
	player.loadFromObjectFile("Objects\\player.obj");

	Vector3 cameraPos{ 75.0f, 60.0f, 25.0f };
	Vector3 cameraDir{ 0.0f, 0.0f, -1.0f };
	Vector3 lightDir{ 0.0f, 0.0f, -1.0f };

	float angleX{ 0.0f };
	float angleY{ 0.0f };

	float speed{ 20.0f };
	float angularSpeed{ 90.0f };

	auto previousTime{ std::chrono::high_resolution_clock::now() };
	while (true)
	{
		auto currentTime{ std::chrono::high_resolution_clock::now() };
		std::chrono::duration<float> deltaTimeDuration{ currentTime - previousTime };
		previousTime = currentTime;
		float deltaTime{deltaTimeDuration.count()};

		Matrix4x4 pointAtMat{ Matrix4x4::pointAt(cameraPos, cameraPos + cameraDir, Vector3{0, 1, 0})};
		Matrix4x4 lookAtMat{ pointAtMat.quickInverse() };

		if (GetAsyncKeyState('X') & 0x8000) break;

		if (GetAsyncKeyState('J') & 0x8000)
		{
			angleY = -angularSpeed;
		}

		if (GetAsyncKeyState('L') & 0x8000)
		{
			angleY = +angularSpeed;
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			Vector3 right{ pointAtMat.getCol(0) };
			cameraPos = cameraPos + right * -(speed * deltaTime);
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			Vector3 right{ pointAtMat.getCol(0) };
			cameraPos = cameraPos + right * (speed * deltaTime);
		}

		if (GetAsyncKeyState('W') & 0x8000)
		{
			Vector3 forward{ pointAtMat.getCol(2) };
			cameraPos = cameraPos + forward * -(speed * deltaTime);
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			Vector3 forward{ pointAtMat.getCol(2) };
			cameraPos = cameraPos + forward * (speed * deltaTime);
		}

		if (GetAsyncKeyState('Q') & 0x8000)
		{
			Vector3 up{ pointAtMat.getCol(1) };
			cameraPos = cameraPos + up * (speed * deltaTime);
		}

		if (GetAsyncKeyState('E') & 0x8000)
		{
			Vector3 up{ pointAtMat.getCol(1) };
			cameraPos = cameraPos + up * -(speed * deltaTime);
		}

		cameraDir = Matrix4x4::rotationY(angleY * deltaTime) * cameraDir;
		angleY = 0.0f;

		renderer.addMeshToWorld(potato_city, Matrix4x4::translation(Vector3{0, 0, 0}) * Matrix4x4::rotationX(180.0f), lightDir);
		renderer.addMeshToWorld(player, pointAtMat * Matrix4x4::translation(Vector3{ 0, -0.5f, -4.5f }) * Matrix4x4::rotationY(180.0f) * Matrix4x4::rotationX(180.0f), lightDir);

		renderer.perspectiveProjection(projectionMat, lookAtMat, cameraPos);

		screen.drawScreenBuffer(renderer.screenBuffer);
		screen.print();
		screen.fill(' ');
		renderer.clearBuffers();
	}

	return 0;
}
