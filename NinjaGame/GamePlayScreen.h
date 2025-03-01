#pragma once

#include <SnakEngine\IGameScreen.h>

#include <Box2D\Box2D.h>
#include <vector>

#include "box.h"
#include "Player.h"

#include <SnakEngine\SWindow.h>
#include <SnakEngine\Camera2D.h>
#include <SnakEngine\GLSLProgram.h>
#include <SnakEngine\GLTexture.h>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\ResourceManager.h>
#include <SnakEngine\DebugRenderer.h>


class CGamePlayScreen : public SnakEngine::IGameScreen
{
public:
	CGamePlayScreen(SnakEngine::SWindow* pWindow);
	virtual ~CGamePlayScreen();

	virtual int GetNextScreenIndex() const override;
	virtual int GetPreviousScreenIndex() const override;

	virtual void Create() override;
	virtual void Destroy() override;

	virtual void OnStart() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Draw() override;

private:
	void UpdateInput();

	std::unique_ptr<b2World> m_pWorld;

	std::vector<CBox> m_cCrates;
	CPlayer m_cPlayer;

	SnakEngine::SpriteBatch m_cSpriteBatch;
	SnakEngine::GLSLProgram m_cShader;
	SnakEngine::Camera2D m_cCamera;
	SnakEngine::GLTexture m_cTexture;
	SnakEngine::SWindow* m_cWindow;
	SnakEngine::CDebugRenderer m_cDebugRenderer;

	bool m_isDebugRender = false;
};

