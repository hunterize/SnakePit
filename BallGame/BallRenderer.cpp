#include "BallRenderer.h"



void CBallRenderer::RenderBall(SnakEngine::SpriteBatch& spriteBatch, CBall& ball)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 destRect(ball.m_cPosition.x - ball.m_fRadius, ball.m_cPosition.y - ball.m_fRadius, ball.m_fRadius * 2.0f, ball.m_fRadius * 2.0f);

	spriteBatch.Draw(destRect, uvRect, ball.m_uiTextureID, 0.0f, ball.m_cColor);
}