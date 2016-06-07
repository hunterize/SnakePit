#include "Bullet.h"



Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed):
	m_cPosition(position), m_cDirection(direction), m_fDamage(damage), m_fSpeed(speed)
{

}


Bullet::~Bullet()
{
}


bool Bullet::Update(const std::vector<std::string>& levelData, float elapseTime)
{
	m_cPosition += m_cDirection * m_fSpeed * elapseTime;
	return CollideWithLevel(levelData);
}


//check collision with agent, DON'T do collision, just return
bool Bullet::CollideWithAgent(Agent* agent)
{
	const float COLLISION_DISTANCE = BULLET_RADIUS + agent->GetRadius();

	glm::vec2 disVec = m_cPosition - (agent->GetPosition() + glm::vec2(agent->GetRadius()));

	float distance = glm::length(disVec);


	//return (COLLISION_DISTANCE - distance);  doesn't work
	if (COLLISION_DISTANCE - distance > 0)
		return true;
	return false;


}

//draw bullet by sprite batch
void Bullet::Draw(SnakEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect(m_cPosition.x, m_cPosition.y, BULLET_RADIUS * 2.0f, BULLET_RADIUS * 2.0f);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	SnakEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 0;
	color.a = 255;

	spriteBatch.Draw(destRect, uvRect, SnakEngine::ResourceManager::GetTexture("Textures/circle.png").ID, 0.0f, color);
}

//simply check the collision with level
bool Bullet::CollideWithLevel(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(m_cPosition.x / (float)TILE_WIDTH);
	gridPosition.y = levelData.size() - 1 - floor(m_cPosition.y / (float)TILE_HEIGHT);


	//go out side the level
	if (gridPosition.x < 0 || gridPosition.x > levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y > levelData.size())
	{
		return true;
	}

	//hit the tile
	return (levelData[gridPosition.y][gridPosition.x] != '.');

}

