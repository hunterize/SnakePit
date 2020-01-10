#include "BallRenderer.h"



void CBallRenderer::RenderBall(SnakEngine::SpriteBatch& spriteBatch, const CBall& ball)
{
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 destRect(ball.m_cPosition.x - ball.m_fRadius, ball.m_cPosition.y - ball.m_fRadius, ball.m_fRadius * 2.0f, ball.m_fRadius * 2.0f);

	spriteBatch.Draw(destRect, uvRect, ball.m_uiTextureID, 0.0f, ball.m_cColor);
}

void CBallRenderer::RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (m_pProgram == nullptr)
	{
		m_pProgram = std::make_unique<SnakEngine::GLSLProgram>();

		m_pProgram->CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_pProgram->AddAttribute("vertexPosition");
		m_pProgram->AddAttribute("vertexColor");
		m_pProgram->AddAttribute("vertexUV");
		m_pProgram->LinkShaders();
	}

	m_pProgram->use();
	
	spriteBatch.Begin();

	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = m_pProgram->GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	GLint pUniform = m_pProgram->GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	for (auto& ball : balls)
	{
		RenderBall(spriteBatch, ball);
	}

	spriteBatch.End();
	spriteBatch.RenderBatch();

	m_pProgram->unuse();

}