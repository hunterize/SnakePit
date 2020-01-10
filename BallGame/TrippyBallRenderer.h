#pragma once
#include "BallRenderer.h"

class CTrippyBallRenderer : public CBallRenderer
{
public:
	CTrippyBallRenderer(int width, int height);
	void RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix) override;
private:
	int m_iScreenWidth;
	int m_iScreenHeight;
	float m_fTime = 0.0f;
};

