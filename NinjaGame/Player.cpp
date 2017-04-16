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
	SnakEngine::GLTexture texture = SnakEngine::ResourceManager::GetTexture("Textures/blue_ninja.png");
	m_cColor = color;
	m_cRenderDimension = renderDimension;
	m_cCollisionCapsule.Init(pWorld, position, collisionDimension, 5.0f, 0.1f, true);
	m_cTileSheet.Init(texture, glm::vec2(10, 2));
}
void CPlayer::Draw(SnakEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;
	b2Body* pBody = m_cCollisionCapsule.GetBody();

	destRect.x = pBody->GetPosition().x - m_cRenderDimension.x / 2.0f;
	destRect.y = pBody->GetPosition().y - m_cCollisionCapsule.GetDimension().y / 2.0f;
	destRect.z = m_cRenderDimension.x;
	destRect.w = m_cRenderDimension.y;

	//calculate the animation

	int tileIndex;
	int numTiles;
	float animationSpeed = 0.2f;

	glm::vec2 velocity;
	velocity.x = pBody->GetLinearVelocity().x;
	velocity.y = pBody->GetLinearVelocity().y;

	//animation on the ground
	if (m_isOnGround)
	{
		//animation when punching
		if (m_isPunching)
		{
			numTiles = 4;
			tileIndex = 1;

			if (m_state != PlayerState::PUNCHING)
			{
				m_state = PlayerState::PUNCHING;
				m_fAnimationTime = 0.0f;
			}
		}
		//animation when running
		else if(abs(velocity.x) > 1.0f && ((velocity.x > 0 && m_iDirection > 0) || (velocity.x < 0 && m_iDirection < 0)))
		{
			numTiles = 6;
			tileIndex = 10;
			animationSpeed = abs(velocity.x) * 0.025f;

			if (m_state != PlayerState::RUNNING)
			{
				m_state = PlayerState::RUNNING;
				m_fAnimationTime = 0.0f;
			}
		}
		//animation when standing still
		else
		{
			numTiles = 1;
			tileIndex = 0;
			
			m_state = PlayerState::STANDING;
		}
	}
	//animation in the air
	else
	{
		//animation when punching
		if (m_isPunching)
		{
			numTiles = 1;
			tileIndex = 18;
			animationSpeed *= 0.25f;

			if (m_state != PlayerState::PUNCHING)
			{
				m_state = PlayerState::PUNCHING;
				m_fAnimationTime = 0.0f;
			}
		}
		//animation when moving in the air
		else if (abs(velocity.x) > 10.0f)
		{
			numTiles = 1;
			tileIndex = 10;

			m_state = PlayerState::IN_AIR;
		}
		//animation when falling
		else if (velocity.y <= 0.0f)
		{
			numTiles = 1;
			tileIndex = 17;

			m_state = PlayerState::IN_AIR;
		}
		//animation when rising
		else
		{
			numTiles = 1;
			tileIndex = 16;
			
			m_state = PlayerState::IN_AIR;
		}
	}

	//calculate animation time
	m_fAnimationTime += animationSpeed;

	if (m_fAnimationTime > numTiles)
	{
		m_isPunching = false;
	}


	tileIndex = tileIndex + (int)m_fAnimationTime % numTiles;
	glm::vec4 uvRect = m_cTileSheet.GetUVs(tileIndex);
	

	//check direction
	if (m_iDirection == -1)
	{
		uvRect.x += 1.0f / m_cTileSheet.GetDimension().x;
		uvRect.z *= -1;
	}

	spriteBatch.Draw(destRect, uvRect, m_cTileSheet.GetTexture().ID, 0.0f, m_cColor, pBody->GetAngle());

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
		pBody->ApplyForceToCenter(b2Vec2(-50.0f, 0.0f), true);
		m_iDirection = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d))
	{
		pBody->ApplyForceToCenter(b2Vec2(50.0f, 0.0f), true);
		m_iDirection = 1;
	}
	else
	{
		pBody->SetLinearVelocity(b2Vec2(pBody->GetLinearVelocity().x, pBody->GetLinearVelocity().y));
	}

	if (inputManager.isKeyDown(SDLK_SPACE))
	{
		m_isPunching = true;
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

	m_isOnGround = false;
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
				if (manifold.points[i].y < pBody->GetPosition().y - m_cCollisionCapsule.GetDimension().y / 2.0f  + m_cCollisionCapsule.GetDimension().x / 2.0f + 0.01f)
				{
					canJump = true;
					break;
				}
			}
			if (canJump)
			{
				m_isOnGround = true;
				if (inputManager.isKeyPressed(SDLK_w))
				{
					pBody->ApplyLinearImpulse(b2Vec2(0.0f, 70.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}




}