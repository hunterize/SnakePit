#pragma once
#include "BallRenderer.h"

class CVelocityBallRenderer : public CBallRenderer
{
public:
	CVelocityBallRenderer(int width, int height);
	void RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix) override;

private:
	int m_iScreenHeight;
	int m_iScreenWidth;
};

