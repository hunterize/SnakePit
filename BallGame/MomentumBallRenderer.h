#pragma once

#include "BallRenderer.h"

class CMomentumBallRenderer : public CBallRenderer
{
public:
	void RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix) override;
};

