#pragma once

#include <SnakEngine\SWindow.h>
#include <SnakEngine\GLSLProgram.h>
#include <SnakEngine\Camera2D.h>
#include <SnakEngine\InputManager.h>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\SpriteFont.h>
#include <SnakEngine\SnakEngine.h>
#include <SnakEngine\Timing.h>
#include <SnakEngine\Errors.h>


#include <iostream>
#include <random>
#include <ctime>

#include "Level.h"
#include "Player.h"
#include "Human.h"
#include "Zombie.h"
#include "Gun.h"

enum class GameState
{
	PLAY,
	WIN,
	LOSE,
	EXIT
};

class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	void Run();

private:
	void InitSystem();
	void InitLevel();
	void InitShaders();
	void GameLoop();
	void ProcessInput();
	void DrawGame();
	void UpdateCamera(float elapseTime);
	void UpdateAgent(float elapseTime);
	void UpdateBullet(float elapseTime);
	void TraceFPS();
	void CheckGameStatus();
	bool IsGameOver();
	void DrawHud();

	SnakEngine::SWindow m_cWindow;
	SnakEngine::GLSLProgram m_cShader;
	SnakEngine::InputManager m_cInputManager;


	SnakEngine::Camera2D m_cCamera;
	SnakEngine::Camera2D m_cHudCamera;

	//for agents
	SnakEngine::SpriteBatch m_cSpriteBatch;

	//game window width and height
	int m_iScreenWidth;
	int m_iScreenHeight;


	//game FPS
	float m_fFPS;

	//game status
	GameState m_eGameState;


	//game levels
	std::vector<Level*> m_cLevels;
	int m_iCurrentLevel;

	//sprites in game
	Player* m_pPlayer;
	std::vector<Human*> m_cHumens;
	std::vector<Zombie*> m_cZombies;
	std::vector<Bullet> m_cBullet;

	//game statistics
	int m_iHumansKilled;
	int m_iZombiesKilled;
	int m_iHumansInfected;

	//sprite font
	SnakEngine::SpriteBatch m_cHudSpriteBatch;
	SnakEngine::SpriteFont* m_pSpriteFont;
};

