#ifndef CONSOLE_SCREEN_H
#define CONSOLE_SCREEN_H

#include "vector3.h"
#include "mesh.h"

#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>

// A simple struct to handle 2D integer screen coordinates
struct ScreenCoord
{
	int x, y;
};

template <int width, int height>
class ConsoleScreen
{
private:
	char data[width * height];

	// Helper: Converts Normalized Device Coordinates (-1 to 1) to Screen Pixels
	ScreenCoord convertScreenCoordinate(const Vector3& p) const
	{
		return {
			static_cast<int>((width - 1) * (p.x + 1) / 2.0f),
			static_cast<int>((height - 1) * (1 - p.y) / 2.0f)
		};
	}

	// Helper: The 2D Cross-Product Edge Function
	// Returns > 0 if point C is on the "inside" of the line AB
	int edgeFunction(const ScreenCoord& a, const ScreenCoord& b, const ScreenCoord& c) const
	{
		return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	}

public:
	ConsoleScreen()
	{
		fill(' ');
	}

	void fill(char ch)
	{
		for (int i{ 0 }; i < width * height; ++i)
			data[i] = ch;
	}

	void set(int x, int y, char ch)
	{
		if (x < 0 || x >= width || y < 0 || y >= height) return;
		data[y * width + x] = ch;
	}

	void drawScreenBuffer(const std::vector<Triangle>& screenBuffer)
	{
		for (const Triangle& tri : screenBuffer)
		{
			// drawTriangle(tri, '#');
			fillTriangle(tri, tri.symbol);
		}
	}

	// Bresenham's line algorithm
	void drawLine(int x0, int y0, int x1, int y1, char ch)
	{
		int dx{ std::abs(x1 - x0) };
		int sx{ x0 < x1 ? 1 : -1 };

		int dy{ -std::abs(y1 - y0) };
		int sy{ y0 < y1 ? 1 : -1 };

		int error{ dx + dy };

		while (true)
		{
			set(x0, y0, ch);
			int e2{ 2 * error };

			if (e2 >= dy)
			{
				if (x0 == x1) break;
				error += dy;
				x0 += sx;
			}

			if (e2 <= dx)
			{
				if (y0 == y1) break;
				error += dx;
				y0 += sy;
			}
		}
	}

	void drawLine(const Vector3& p0, const Vector3& p1, char ch)
	{
		ScreenCoord v0 = convertScreenCoordinate(p0);
		ScreenCoord v1 = convertScreenCoordinate(p1);

		drawLine(v0.x, v0.y, v1.x, v1.y, ch);
	}

	void drawTriangle(const Triangle& tri, char ch)
	{
		drawLine(tri.vertices[0], tri.vertices[1], ch);
		drawLine(tri.vertices[1], tri.vertices[2], ch);
		drawLine(tri.vertices[2], tri.vertices[0], ch);
	}

	// Modern Barycentric Fill Algorithm
	void fillTriangle(const Triangle& tri, char ch)
	{
		// 1. Convert vertices to screen space
		ScreenCoord v0 = convertScreenCoordinate(tri.vertices[0]);
		ScreenCoord v1 = convertScreenCoordinate(tri.vertices[1]);
		ScreenCoord v2 = convertScreenCoordinate(tri.vertices[2]);

		// 2. Find the Bounding Box (and clamp it to the screen boundaries)
		int minX = std::max(0, std::min({ v0.x, v1.x, v2.x }));
		int minY = std::max(0, std::min({ v0.y, v1.y, v2.y }));
		int maxX = std::min(width - 1, std::max({ v0.x, v1.x, v2.x }));
		int maxY = std::min(height - 1, std::max({ v0.y, v1.y, v2.y }));

		// 3. Loop through every pixel inside the bounding box
		ScreenCoord p;
		for (p.y = minY; p.y <= maxY; ++p.y)
		{
			for (p.x = minX; p.x <= maxX; ++p.x)
			{
				// Calculate Barycentric weights (Distance from each edge)
				int w0 = edgeFunction(v1, v2, p);
				int w1 = edgeFunction(v2, v0, p);
				int w2 = edgeFunction(v0, v1, p);

				// If the point is on the inside of all three edges, draw it!
				if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
					(w0 <= 0 && w1 <= 0 && w2 <= 0))
				{
					set(p.x, p.y, ch);
				}
			}
		}
	}

	void print()
	{
		std::string output = "\033[H";
		output.reserve(output.size() + height * (width * 2 + 1));

		for (int i{ 0 }; i < height; ++i)
		{
			for (int j{ 0 }; j < width; ++j)
			{
				output += data[i * width + j];
				output += ' ';
			}
			output += '\n';
		}

		std::cout.write(output.data(), output.size());
	}

	constexpr float aspect_ratio() const
	{
		return static_cast<float>(width) / height;
	}
};

#endif