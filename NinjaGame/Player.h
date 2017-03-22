#pragma once
#include "capsule.h"
#include <SnakEngine\InputManager.h>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\GLTexture.h>

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void Init(b2World* pWorld, 
		const glm::vec2& position, 
		const glm::vec2& renderDimension,
		glm::vec2& collisionDimension,
		SnakEngine::Color color);
	void Draw(SnakEngine::SpriteBatch& spriteBatch);
	void DrawDebug(SnakEngine::CDebugRenderer& debugRenderer);
	void Update(SnakEngine::InputManager& const inputManager);
	const CCapsule& GetCapsule() const { return m_cCollisionCapsule; }

private:
	CCapsule m_cCollisionCapsule;
	glm::vec2 m_cRenderDimension;
	SnakEngine::Color m_cColor;
	SnakEngine::GLTexture m_cTexture;
};

