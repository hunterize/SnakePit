#pragma once

#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\GLSLProgram.h>
#include "ball.h"

#include <memory>


class CBallRenderer
{
public:
	void RenderBall(SnakEngine::SpriteBatch& spriteBatch, const CBall& ball);
	virtual void RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix);

protected:
	std::unique_ptr<SnakEngine::GLSLProgram> m_pProgram = nullptr;
};

