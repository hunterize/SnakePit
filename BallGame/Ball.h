#pragma once

#include <glm\glm.hpp>
#include <SnakEngine\Vertex.h>

struct CBall
{
	CBall(float radius, float mass, const glm::vec2& pos, const glm::vec2& vel, unsigned int textureid, const SnakEngine::Color& color);

	float m_fRadius;
	float m_fMass;
	glm::vec2 m_cVelocity;
	glm::vec2 m_cPosition;
	unsigned int m_uiTextureID = 0;
	SnakEngine::Color m_cColor;
};

