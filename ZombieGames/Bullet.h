#pragma once
#include <glm\glm.hpp>

#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\ResourceManager.h>

#include "Level.h"
#include "Agent.h"

const float BULLET_RADIUS = 5.0f;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	bool Update(const std::vector<std::string>& levelData, float elapseTime);
	bool CollideWithAgent(Agent* agent);

	void Draw(SnakEngine::SpriteBatch& spriteBatch);

	float GetDamage() { return m_fDamage; }

private:

	bool CollideWithLevel(const std::vector<std::string>& levelData);

	glm::vec2 m_cPosition;
	glm::vec2 m_cDirection;
	float m_fDamage;
	float m_fSpeed;

	//const float BULLET_RADIUS;

};

