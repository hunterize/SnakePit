#pragma once

#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\ResourceManager.h>
#include "Level.h"
#include <algorithm>

struct CollisionTile
{
	glm::vec2 tilePosition;
    static glm::vec2 agentPosition;
};

class Agent
{
public:
	Agent();
	virtual ~Agent();
	void Draw(SnakEngine::SpriteBatch& spriteBatch);
	virtual void Update(const std::vector<std::string>& levelData, float elapseTime) = 0;
	glm::vec2 GetPosition() const { return m_cPosition; }
	bool CollideWithAgent(Agent* agent);
	float GetRadius() { return AGENT_RADIUS; }

	bool ApplyDemage(float damage);
	float GetHealth() { return m_fHealth; }

protected:
	void CheckTilePosition(const std::vector<std::string>& levelData, float x, float y);
	void CollideWithTile(glm::vec2 tilePos);
	static bool CompareCollideTile(CollisionTile t1, CollisionTile t2);
	bool CollideWithLevel(const std::vector<std::string>& levelData);

	std::vector<CollisionTile> m_cCollisionBox;
	glm::vec2 m_cPosition;
	glm::vec2 m_cDirection = glm::vec2(1.0f, 0.0f);

	SnakEngine::Color m_cColor;
	float m_fSpeed;
	float m_fHealth;
	const float AGENT_RADIUS;
	GLuint m_iTextureID;
};

