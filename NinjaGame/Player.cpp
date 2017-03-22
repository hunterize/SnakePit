#include "Player.h"
#include <SnakEngine\ResourceManager.h>
#include <SDL\SDL.h>


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init(b2World* pWorld, 
	const glm::vec2& position, 
	const glm::vec2& renderDimension,
	glm::vec2& collisionDimension,
	SnakEngine::Color color)
{
	m_cTexture = SnakEngine::ResourceManager::GetTexture("Textures/blue_ninja.png");
	m_cColor = color;
	m_cRenderDimension = renderDimension;
	m_cCollisionCapsule.Init(pWorld, position, collisionDimension, 5.0f, 0.1f, true);
}
void CPlayer::Draw(SnakEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;
	b2Body* pBody = m_cCollisionCapsule.GetBody();

	destRect.x = pBody->GetPosition().x - m_cRenderDimension.x / 2.0f;
	destRect.y = pBody->GetPosition().y - m_cCollisionCapsule.GetDimension().y / 2.0f;
	destRect.z = m_cRenderDimension.x;
	destRect.w = m_cRenderDimension.y;
	spriteBatch.Draw(destRect, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f), m_cTexture.ID, 0.0f, m_cColor, pBody->GetAngle());

}

void CPlayer::DrawDebug(SnakEngine::CDebugRenderer& debugRenderer)
{
	m_cCollisionCapsule.DrawDebug(debugRenderer);
}

void CPlayer::Update(SnakEngine::InputManager& inputManager)
{
	static const float MAXSPEED = 10.0f;
	b2Body* pBody = m_cCollisionCapsule.GetBody();

	if (inputManager.isKeyDown(SDLK_a))
	{
		pBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d))
	{
		pBody->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	else
	{
		pBody->SetLinearVelocity(b2Vec2(pBody->GetLinearVelocity().x, pBody->GetLinearVelocity().y));
	}

	//limit the speed
	if (pBody->GetLinearVelocity().x < -MAXSPEED)
	{
		pBody->SetLinearVelocity(b2Vec2(-MAXSPEED, pBody->GetLinearVelocity().y));
	}
	if (pBody->GetLinearVelocity().x > MAXSPEED)
	{
		pBody->SetLinearVelocity(b2Vec2(MAXSPEED, pBody->GetLinearVelocity().y));
	}

	for (b2ContactEdge* ce = pBody->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Contact* c = ce->contact;
		if (c->IsTouching())
		{
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);

			bool canJump = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (manifold.points[i].y < pBody->GetPosition().y - m_cCollisionCapsule.GetDimension().y / 2.0f + 0.01f)
				{
					canJump = true;
					break;
				}
			}
			if (canJump)
			{
				if (inputManager.isKeyPressed(SDLK_w))
				{
					pBody->ApplyLinearImpulse(b2Vec2(0.0f, 100.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}




}