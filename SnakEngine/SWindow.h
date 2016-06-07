#pragma once

#include <SDL\SDL.h>
#include <GLEW\glew.h>
#include <string>

namespace SnakEngine
{
	enum WindowStyle { INVISIBLE = 0x1, FULLSCREEN = 0x2, BOARDLESS = 0x4 };

	class SWindow
	{
	public:
		SWindow();
		~SWindow();

		void Create(std::string windowName, int windowWidth, int windowHeight, Uint32 windowStyle);
		void SwapBuffer();

		int GetWidth() { return m_iWidth; }
		int GetHeight() { return m_iHeight; }

	private:
		SDL_Window* m_pWindow;
		int m_iWidth;
		int m_iHeight;

	};
}
