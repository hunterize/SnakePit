#include "Agent.h"

glm::vec2 CollisionTile::agentPosition = glm::vec2(0.0f);

Agent::Agent() : AGENT_RADIUS(25.0f)
{
}


Agent::~Agent()
{
}

void Agent::Draw(SnakEngine::SpriteBatch& spriteBatch)
{
	static int textureID = SnakEngine::ResourceManager::GetTexture("Textures/circle.png").ID;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_cPosition.x;
	destRect.y = m_cPosition.y;
	destRect.z = AGENT_RADIUS * 2;
	destRect.w = AGENT_RADIUS * 2;
	
	spriteBatch.Draw(destRect, uvRect, textureID, 0.0f, m_cColor);
}

bool Agent::ApplyDemage(float damage)
{
	m_fHealth -= damage;
	if (m_fHealth <= 0.0f)
	{
		return true;
	}
	return false;
}


bool Agent::CollideWithAgent(Agent* agent)
{
	//minimum distance of collision of two agents
	const float COLLISION_DISTANCE = AGENT_RADIUS * 2.0f;

	//get the distance vector from this to agent
	glm::vec2 disVec = m_cPosition - agent->m_cPosition;

	//distance of two agents
	float dis = glm::length(disVec);

	float collisionDepth = COLLISION_DISTANCE - dis;

	//check if dis > 0 to avoid NAN
	if (collisionDepth > 0 && dis > 0)
	{
		
		//collision depth vector, normalize returns (x/sqrt(x*x + y*y), y/sqrt(x*x + y*y)) with same direction
		glm::vec2 collisionDepthVector = glm::normalize(disVec) * collisionDepth;
		bool isNAN = isnan(collisionDepthVector.x);
		m_cPosition += collisionDepthVector / 2.0f;
		agent->m_cPosition -= collisionDepthVector / 2.0f;
		return true;
	}
	return false;

}

//AABB collision detection
//create AABB collision box - tiles in vector
bool Agent::CollideWithLevel(const std::vector<std::string>& levelData)
{
	//it's called when updating, clear AABB box for every frame
	if (m_cCollisionBox.size())
	{
		m_cCollisionBox.clear();
	}

	//the first corner, bottom left
	CheckTilePosition(levelData, m_cPosition.x, m_cPosition.y);

	//the second corner, bottom right
	CheckTilePosition(levelData, m_cPosition.x + AGENT_RADIUS * 2.0f, m_cPosition.y);

	//the third corner, top right
	CheckTilePosition(levelData, m_cPosition.x, m_cPosition.y + AGENT_RADIUS * 2.0f);

	//the fourth corner, top left
	CheckTilePosition(levelData, m_cPosition.x + AGENT_RADIUS * 2.0f, m_cPosition.y + AGENT_RADIUS * 2.0f);

	//no collision tiles
	if (m_cCollisionBox.size() == 0)
		return false;


	if (m_cCollisionBox.size() == 2)
	{
		std::stable_sort(m_cCollisionBox.begin(), m_cCollisionBox.end(), CompareCollideTile);
	}

	//do collision
	for each (CollisionTile tile in m_cCollisionBox)
	{
		CollideWithTile(tile.tilePosition);
	}


	return true;
}

bool Agent::CompareCollideTile(CollisionTile t1, CollisionTile t2)
{

	return (glm::distance(t1.agentPosition, t1.tilePosition) < glm::distance(t2.agentPosition, t2.tilePosition));
	//if(t1.tilePosition.y == t2.tilePosition.y)
	//return (std::abs(t1.tilePosition.x - t1.agentPosition.x) < std::abs(t2.tilePosition.x - t2.agentPosition.x));

	//if (t1.tilePosition.x == t2.tilePosition.x)
	//	return (std::abs(t1.tilePosition.y - t1.agentPosition.y) < std::abs(t2.tilePosition.y - t2.agentPosition.y));

}



///protected method

//get collistion tile position which contains the point (x, y), the agent position is the at the left bottom corner of the texture, 
//save tile position as the center of the tile
void Agent::CheckTilePosition(const std::vector<std::string>& levelData, float x, float y)
{
	//get the postion of this corner in level data (grid space),  the level data line is from top to bottom in level file
	glm::vec2 gridPos = glm::vec2(floor(x / (float)TILE_WIDTH), levelData.size() - 1 - floor(y / (float)TILE_HEIGHT));

	if (gridPos.x < 0 || gridPos.y >= levelData[0].size() || gridPos.y < 0 || gridPos.y >= levelData.size())
	{
		return;
	}

	//'.' stands for air, notice the position x indicates column
	if (levelData[gridPos.y][gridPos.x] != '.')
	{
		//save collidebox tiles in which tile and agent positions are all centred
		//collideTilePostion.push_back(gridPos * glm::vec2((float)TILE_WIDTH, (float)TILE_HEIGHT) + glm::vec2((float)TILE_WIDTH/2.0f, (float)TILE_HEIGHT/2.0f));
		//same as level renders, need to consider the top to bottom issue
		CollisionTile tmpTile;
		tmpTile.tilePosition = glm::vec2(gridPos.x * TILE_WIDTH, (levelData.size() - 1 - gridPos.y) * TILE_HEIGHT) + glm::vec2((float)TILE_WIDTH / 2.0f, (float)TILE_HEIGHT / 2.0f);
		tmpTile.agentPosition = m_cPosition + glm::vec2(AGENT_RADIUS);

		m_cCollisionBox.push_back(tmpTile);
	
		//m_cCollisionBox.push_back({ glm::vec2(gridPos.x * TILE_WIDTH, (levelData.size() - 1 - gridPos.y) * TILE_HEIGHT) + glm::vec2((float)TILE_WIDTH / 2.0f, (float)TILE_HEIGHT / 2.0f),
		//							m_cPosition	+ glm::vec2(AGENT_RADIUS)
		//							});
	}
}


//collide with AABB 
//move backforward by minimum depth (x or y) when collision happens 
void Agent::CollideWithTile(glm::vec2 tilePos)
{
	const float CLOSEST_X_DISTANCE = TILE_WIDTH / 2.0f + AGENT_RADIUS;
	const float CLOSEST_Y_DISTANCE = TILE_HEIGHT / 2.0f + AGENT_RADIUS;

	//center position of the agent
	glm::vec2 agentPos = m_cPosition + glm::vec2(AGENT_RADIUS);
	//get distance vector from agent to tile
	glm::vec2 disVec = agentPos - tilePos;

	//get depth of the collision
	float xDepth = CLOSEST_X_DISTANCE - abs(disVec.x);
	float yDepth = CLOSEST_Y_DISTANCE - abs(disVec.y);

	
	if (xDepth > 0 && yDepth > 0)
	{
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			disVec.x < 0 ? m_cPosition.x -= xDepth : m_cPosition.x += xDepth;
		}
		else
		{
			disVec.y < 0 ? m_cPosition.y -= yDepth : m_cPosition.y += yDepth;
		}
	}


}