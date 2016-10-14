#pragma once

#include <glm\glm.hpp>
#include <SnakEngine\Vertex.h>

#include <random>


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


struct CBallSpawn
{
	CBallSpawn(const SnakEngine::Color& c,
				float r, float m, float minSpeed, float maxSpeed, float prob) :
				color(c),radius(r), mass(m),
				speed(minSpeed, maxSpeed),
				probability(prob) {}

	SnakEngine::Color color;
	float radius;
	float mass;
	float probability;
	std::uniform_real_distribution<float> speed;
};
