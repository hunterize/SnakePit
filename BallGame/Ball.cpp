#include "Ball.h"



CBall::CBall(float radius, float mass, const glm::vec2& pos, const glm::vec2& vel, unsigned int textureid, const SnakEngine::Color& color)
{
	m_fRadius = radius;
	m_fMass = mass;
	m_cVelocity = vel;
	m_cPosition = pos;
	m_uiTextureID = textureid;
	m_cColor = color;
}


