

#include "SnakEngine.h"

#include <SDL\SDL.h>
#include <GLEW\glew.h>

namespace SnakEngine
{
	void InitEngine()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		//set double buffer to avoid flicker
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

}