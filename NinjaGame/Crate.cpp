#include "Crate.h"



CCrate::CCrate()
{
}


CCrate::~CCrate()
{
}

void CCrate::Init(b2World* pWorld, const glm::vec2& position, const glm::vec2& dimension, SnakEngine::Color color)
{
	m_cDimension = dimension;
	m_cColor = color;

	b2BodyDef crateBody;
	crateBody.type = b2_dynamicBody;
	crateBody.position.Set(position.x, position.y);
	m_pBody = pWorld->CreateBody(&crateBody);

	b2PolygonShape crateShape;
	crateShape.SetAsBox(dimension.x / 2.0f, dimension.y / 2.0f);
	b2FixtureDef crateFixture;
	crateFixture.shape = &crateShape;
	crateFixture.density = 1.0f;
	crateFixture.friction = 0.3f;
	m_pFixture = m_pBody->CreateFixture(&crateFixture);
}

