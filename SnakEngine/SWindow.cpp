#include "SWindow.h"
#include "Errors.h"

#include <iostream>

namespace SnakEngine
{
	SWindow::SWindow()
	{
	}


	SWindow::~SWindow()
	{
	}

	void SWindow::Create(std::string windowName, int windowWidth, int windowHeight, Uint32 windowStyle)
	{
		Uint32 flag = SDL_WINDOW_OPENGL;
		if (windowStyle & WindowStyle::INVISIBLE)
		{
			flag |= SDL_WINDOW_HIDDEN;
		}

		if (windowStyle & WindowStyle::FULLSCREEN)
		{
			flag |= SDL_WINDOW_FULLSCREEN;
		}

		if (windowStyle & WindowStyle::BOARDLESS)
		{
			flag |= SDL_WINDOW_BORDERLESS;
		}

		m_pWindow = SDL_CreateWindow(
			windowName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth, windowHeight,
			flag);

		if (m_pWindow == nullptr)
		{
			fatalError("SDL Window could not be created");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(m_pWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL_GL context could not be created");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			fatalError("Could not initialize GLEW");
		}
		//check OpenGL version
		std::cout << "*** OpenGL Version is: " << glGetString(GL_VERSION) << std::endl;

		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

		//set VSYNC by 1. off by 0
		SDL_GL_SetSwapInterval(0);


		//Enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SWindow::SwapBuffer()
	{
		SDL_GL_SwapWindow(m_pWindow);
	}
}