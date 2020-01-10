#pragma once

#include <GLEW\glew.h>

namespace SnakEngine
{
	struct Position
	{
		float x;
		float y;
	};

	class Color
	{
	public:
		Color() {};
		Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha):
			r(red), g(green), b(blue), a(alpha)
		{}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV
	{
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;
		Color color;
		UV uv;

		void SetPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void SetUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};
}