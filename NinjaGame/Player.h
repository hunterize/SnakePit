#pragma once
#include "capsule.h"
#include <SnakEngine\InputManager.h>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\GLTexture.h>
#include <SnakEngine\TileSheet.h>

enum class PlayerState {STANDING, RUNNING, PUNCHING, IN_AIR};


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
	SnakEngine::CTileSheet m_cTileSheet;

	PlayerState m_state = PlayerState::STANDING;
	float m_fAnimationTime = 0.0f;
	int m_iDirection = 1;
	bool m_isOnGround = false;
	bool m_isPunching = false;
};

