#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\DebugRenderer.h>


class CCapsule
{
public:
	void Init(b2World* pWorld,
		const glm::vec2& position,
		const glm::vec2& dimension,
		float density,
		float friction,
		bool isFixedRotation);
	
	void DrawDebug(SnakEngine::CDebugRenderer& debugRenderer);
	b2Body* GetBody() { return m_pBody; }
	b2Fixture* GetFixture(int index) const { return m_pFixtures[index]; }
	const glm::vec2& GetDimension() const { return m_cDimension; }

private:
	b2Body* m_pBody = nullptr;
	b2Fixture* m_pFixtures[3];
	glm::vec2 m_cDimension;

};

