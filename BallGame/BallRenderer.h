#pragma once

#include <SnakEngine\SpriteBatch.h>
#include "ball.h"


class CBallRenderer
{
public:
	void RenderBall(SnakEngine::SpriteBatch& spriteBatch, CBall& ball);
};

