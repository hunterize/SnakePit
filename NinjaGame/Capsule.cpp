#include "Capsule.h"

void CCapsule::Init(b2World* pWorld,
	const glm::vec2& position,
	const glm::vec2& dimension,
	float density,
	float friction,
	bool isFixedRotation)
{
	m_cDimension = dimension;
	//create body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = isFixedRotation;
	m_pBody = pWorld->CreateBody(&bodyDef);

	//create box fixture
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimension.x / 2.0f, (dimension.y - dimension.x) / 2.0f);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = density;
	boxFixtureDef.friction = friction;
	m_pFixtures[0] = m_pBody->CreateFixture(&boxFixtureDef);

	//create circle fixture
	b2CircleShape circleShape;
	circleShape.m_radius = dimension.x / 2.0f;

	b2FixtureDef circleFixtureDef;
	circleFixtureDef.shape = &circleShape;
	circleFixtureDef.density = density;
	circleFixtureDef.friction = friction;

	//create bottom circle fixture
	circleShape.m_p.Set(0.0f, -(m_cDimension.y - m_cDimension.x) / 2.0f);
	m_pFixtures[1] = m_pBody->CreateFixture(&circleFixtureDef);

	//create top circle fixture
	circleShape.m_p.Set(0.0f, (m_cDimension.y - m_cDimension.x) / 2.0f);
	m_pFixtures[2] = m_pBody->CreateFixture(&circleFixtureDef);

}

void CCapsule::DrawDebug(SnakEngine::CDebugRenderer& debugRenderer)
{
	SnakEngine::Color color(255,255,255,255);

	//draw box
	glm::vec4 destRect;
	destRect.x = m_pBody->GetPosition().x - m_cDimension.x / 2.0f;
	destRect.y = m_pBody->GetPosition().y - (m_cDimension.y - m_cDimension.x) / 2.0f;
	destRect.z = m_cDimension.x;
	destRect.w = m_cDimension.y - m_cDimension.x;
	debugRenderer.DrawBox(destRect, color, m_pBody->GetAngle());

	//draw circle
	debugRenderer.DrawCircle(glm::vec2(m_pBody->GetPosition().x, m_pBody->GetPosition().y - (m_cDimension.y - m_cDimension.x) / 2.0f), color, m_cDimension.x / 2.0f);
	debugRenderer.DrawCircle(glm::vec2(m_pBody->GetPosition().x, m_pBody->GetPosition().y + (m_cDimension.y - m_cDimension.x) / 2.0f), color, m_cDimension.x / 2.0f);

}
