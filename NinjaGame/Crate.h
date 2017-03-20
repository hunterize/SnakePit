#pragma once

#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <SnakEngine\Vertex.h>

class CCrate
{
public:
	CCrate();
	~CCrate();

	void Init(b2World* pWorld, const glm::vec2& position, const glm::vec2& dimension, SnakEngine::Color color);

	b2Body* GetBody() { return m_pBody; }
	b2Fixture* GetFixture() { return m_pFixture; }
	const glm::vec2& GetDimension() const { return m_cDimension; }
	const SnakEngine::Color& GetColor() const { return m_cColor;}


private:
	b2Body* m_pBody = nullptr;
	b2Fixture* m_pFixture = nullptr;
	glm::vec2 m_cDimension;
	
	SnakEngine::Color m_cColor;
};

