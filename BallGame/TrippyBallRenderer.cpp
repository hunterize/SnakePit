#include "TrippyBallRenderer.h"



CTrippyBallRenderer::CTrippyBallRenderer(int width, int height) : m_iScreenWidth(width), m_iScreenHeight(height)
{
}


void CTrippyBallRenderer::RenderBalls(SnakEngine::SpriteBatch& spriteBatch, const std::vector<CBall>& balls, const glm::mat4& projectionMatrix)
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

	const float TIME_SPEED = 0.01f;
	const float DIVISOR = 4.0f;
	const float SPIRAL_INTENSITY = 10.0f;
	const float PI = 3.14159f;

	m_fTime += TIME_SPEED;

	for (auto& ball : balls)
	{
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.m_cPosition.x - ball.m_fRadius, ball.m_cPosition.y - ball.m_fRadius, ball.m_fRadius * 2.0f, ball.m_fRadius * 2.0f);

		SnakEngine::Color color;

		glm::vec2 clockVector = ball.m_cPosition - glm::vec2(m_iScreenWidth / 2, m_iScreenHeight / 2);
		float clockLength = glm::length(clockVector);
		
		float angle = atan2(clockVector.x, clockVector.y) / (PI / DIVISOR);
		angle -= m_fTime;

		angle += (clockLength / m_iScreenWidth) * SPIRAL_INTENSITY;

		color.r = (GLubyte)(255 * angle);
		color.g = (GLubyte)(255 * angle * cos(m_fTime));
		color.b = (GLubyte)(255 * angle * sin(m_fTime));
		color.a = (GLubyte)(255 * glm::clamp(1.0f - (clockLength / (m_iScreenWidth / 2)), 0.0f, 1.0f));

		spriteBatch.Draw(destRect, uvRect, ball.m_uiTextureID, 0.0f, color);
	}

	spriteBatch.End();
	spriteBatch.RenderBatch();

	m_pProgram->unuse();
}