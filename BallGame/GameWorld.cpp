#include "GameWorld.h"




CGameWorld::CGameWorld()
{
}


CGameWorld::~CGameWorld()
{
}

void CGameWorld::Run()
{
	InitSystem();

	InitBalls();

	GameLoop();

}

void CGameWorld::InitSystem()
{
	SnakEngine::InitEngine();

	m_iScreenHeight = 600;
	m_iScreenWidth = 800;

	m_cWindow.Create("BallGame", m_iScreenWidth, m_iScreenHeight, 0);

	//grey background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//initialize spritebatch
	m_cSpriteBatch.Init();

	//initialize sprite font
	//m_pSpriteFont = new SnakEngine::SpriteFont("Fonts/chintzy.ttf", 64);
	m_pSpriteFont = std::make_unique<SnakEngine::SpriteFont>("Fonts/chintzy.ttf", 64);

	//initialize camera
	m_cCamera.Init(m_iScreenWidth, m_iScreenHeight);
	m_cCamera.SetPosition(glm::vec2(m_iScreenWidth / 2.0f, m_iScreenHeight / 2.0f));

	//initialize shaders
	InitShaders();

	//initialize renders
	InitRenderers();

}

void CGameWorld::InitShaders()
{
	m_cShader.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_cShader.AddAttribute("vertexPosition");
	m_cShader.AddAttribute("vertexColor");
	m_cShader.AddAttribute("vertexUV");
	m_cShader.LinkShaders();
}

void CGameWorld::InitRenderers()
{
}
void CGameWorld::InitBalls()
{}
void CGameWorld::GameLoop()
{
	SnakEngine::FpsLimiter fpsLimiter;
	fpsLimiter.SetTargetFPS(0.0f);
	
	float timeSpan = 0.0f;

	//maximum value of physic time
	float dt = 1 / 15.0;

	while (m_eGameState == GameState::PLAY)
	{
		fpsLimiter.Begin();
		timeSpan = fpsLimiter.GetFrameTime() / 1000.0f;

		ProcessInput();

		//semi-fixed time step
		while (timeSpan > 0.0)
		{
			float deltaTime = std::min(timeSpan, dt);
			Update(deltaTime);
			timeSpan -= deltaTime;
		}

		DrawGame();
		m_fFPS = fpsLimiter.End();
	}


}
void CGameWorld::Update(float eclapseTime)
{}
void CGameWorld::DrawGame()
{}
void CGameWorld::DrawHud()
{}
void CGameWorld::ProcessInput()
{
	SDL_Event event;
	m_cInputManager.Update();

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_eGameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			m_cInputManager.PressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_cInputManager.ReleaseKey(event.key.keysym.sym);
			break;
		default:
			break;
		}
	}

	if (m_cInputManager.isKeyDown(SDLK_ESCAPE))
	{
		m_eGameState = GameState::EXIT;
	}
}