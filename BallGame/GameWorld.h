#pragma once

#include <SnakEngine\SnakEngine.h>
#include <SnakEngine\Camera2D.h>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\InputManager.h>
#include <SnakEngine\SWindow.h>
#include <SnakEngine\GLSLProgram.h>
#include <SnakEngine\Timing.h>
#include <SnakEngine\SpriteFont.h>
#include <SnakEngine\ResourceManager.h>

#include <SDL\SDL.h>

#include <memory>

#include "ballrenderer.h"
#include "MomentumBallRenderer.h"
#include "VelocityBallRenderer.h"
#include "TrippyBallRenderer.h"
#include "ballcontroller.h"
#include "Grid.h"

enum class GameState
{
	PLAY,
	WIN,
	LOSE,
	EXIT
};

class CGameWorld
{
public:
	CGameWorld();
	~CGameWorld();

	void Run();

private:
	void InitSystem();
	void InitRenderers();
	void InitBalls();
	void Update(float eclapseTime);
	void GameLoop();
	void DrawGame();
	void DrawHud();
	void ProcessInput();

	void InitShaders();

	int m_iScreenWidth = 0;
	int m_iScreenHeight = 0;

	std::vector<CBall> m_cBalls;
	std::unique_ptr<CGrid> m_pGrid;

	//CBallRenderer m_cBallRender;
	
	int m_iCurrentRender = 0;
	std::vector<std::unique_ptr<CBallRenderer>> m_ballRenderers;

	CBallController m_cBallController;

	SnakEngine::SWindow m_cWindow;
	SnakEngine::SpriteBatch m_cSpriteBatch;
	//SnakEngine::SpriteFont* m_pSpriteFont;
	std::unique_ptr<SnakEngine::SpriteFont> m_pSpriteFont;
	SnakEngine::Camera2D m_cCamera;
	SnakEngine::InputManager m_cInputManager;
	SnakEngine::GLSLProgram m_cShader;
	SnakEngine::FpsLimiter m_cFPS;

	float m_fFPS = 0.0f;

	GameState m_eGameState = GameState::PLAY;




};

