#pragma once

#include <sdl\SDL.h>
#include <glew\glew.h>

#include <SnakEngine\Sprite.h>
#include <SnakEngine\GLSLProgram.h>
#include <SnakEngine\GLTexture.h>
#include <SnakEngine\SWindow.h>
#include <SnakEngine\Errors.h>
#include <SnakEngine\ImageLoader.h>
#include <SnakEngine\SnakEngine.h>
#include <SnakEngine\Camera2D.h>
#include <SnakEngine\ResourceManager.h>
#include <SnakEngine\SpriteBatch.h>
#include <SnakEngine\InputManager.h>
#include <SnakEngine/Timing.h>
#include <vector>

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	void TraceFPS();

	SnakEngine::SWindow m_cWindow;
	int m_iScreenWidth;
	int m_iScreenHeight;
	
	GameState m_eState;
	//Sprite m_cSprite;


	/////vector.push_back call distruction function to delete the buffer
	std::vector<SnakEngine::Sprite*> m_cSprites;

	
	SnakEngine::SpriteBatch m_cSpriteBatch;

	SnakEngine::InputManager m_cInputManager;

	SnakEngine::GLSLProgram m_cColorProgram;
	//GLTexture m_cPlayerTexture;

	SnakEngine::Camera2D m_cCamera;

	SnakEngine::FpsLimiter m_cFPSLimiter;


	float m_fFPS;
	float m_fMaxFPS;



};

