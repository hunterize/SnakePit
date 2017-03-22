#include "box.h"



CBox::CBox()
{
}


CBox::~CBox()
{
}

void CBox::Init(b2World* pWorld,
	const glm::vec2& position, 
	const glm::vec2& dimension, 
	SnakEngine::GLTexture texture, 
	SnakEngine::Color color, 
	bool isFixedRotation,
	float density,
	glm::vec4 uvRect)
{
	m_cDimension = dimension;
	m_cColor = color;
	m_cTexture = texture;
	m_cUVRect = uvRect;

	b2BodyDef crateBody;
	crateBody.type = b2_dynamicBody;
	crateBody.position.Set(position.x, position.y);
	crateBody.fixedRotation = isFixedRotation;
	m_pBody = pWorld->CreateBody(&crateBody);

	b2PolygonShape crateShape;
	crateShape.SetAsBox(dimension.x / 2.0f, dimension.y / 2.0f);
	b2FixtureDef crateFixture;
	crateFixture.shape = &crateShape;
	crateFixture.density = density;
	crateFixture.friction = 0.3f;
	m_pFixture = m_pBody->CreateFixture(&crateFixture);
}

void CBox::Draw(SnakEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;
	destRect.x = m_pBody->GetPosition().x - m_cDimension.x / 2.0f;
	destRect.y = m_pBody->GetPosition().y - m_cDimension.y / 2.0f;
	destRect.z = m_cDimension.x;
	destRect.w = m_cDimension.y;

	spriteBatch.Draw(destRect, m_cUVRect, m_cTexture.ID, 0.0f, m_cColor, m_pBody->GetAngle());
}

