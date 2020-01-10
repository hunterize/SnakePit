#include "GameWorld.h"
#include <ctime>



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

	m_iScreenHeight = 800;
	m_iScreenWidth = 1600;

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
	m_cCamera.SetPosition(glm::vec2(m_iScreenWidth / 2, m_iScreenHeight / 2));
	m_cCamera.Update();

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
	m_ballRenderers.push_back(std::make_unique<CBallRenderer>());
	m_ballRenderers.push_back(std::make_unique<CMomentumBallRenderer>());
	m_ballRenderers.push_back(std::make_unique<CVelocityBallRenderer>(m_iScreenWidth, m_iScreenHeight));
	m_ballRenderers.push_back(std::make_unique<CTrippyBallRenderer>(m_iScreenWidth, m_iScreenHeight));
}


void CGameWorld::InitBalls()
{
	const int CELLSIZE = 14;

	m_pGrid = std::make_unique<CGrid>(m_iScreenWidth, m_iScreenHeight, CELLSIZE);

	const int NUM_BALLS = 20000;

	//initialize random engine
	std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> randX(0.0f, (float)m_iScreenWidth);
	std::uniform_real_distribution<float> randY(0.0f, (float)m_iScreenHeight);
	std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	//initialize possible ball spawn
	std::vector<CBallSpawn> possibleBalls;
	float fTotalProbability = 0.0f;

#define ADD_BALL(p, ...) \
	fTotalProbability += p; \
	possibleBalls.emplace_back(__VA_ARGS__);

	ADD_BALL(20.0f, SnakEngine::Color(0, 255, 0, 255), 2.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(10.0f, SnakEngine::Color(0, 0, 255, 255), 4.0f, 2.0f, 10.0f, 15.0f, fTotalProbability);
	ADD_BALL(5.0f, SnakEngine::Color(255, 0, 0, 255), 6.0f, 4.0f, 30.0f, 40.0f, fTotalProbability);
	ADD_BALL(10.0f, SnakEngine::Color(125, 125, 125, 255), 2.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(20.0f, SnakEngine::Color(0, 255, 255, 255), 2.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(15.0f, SnakEngine::Color(255, 0, 255, 255), 4.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(5.0f, SnakEngine::Color(0, 255, 255, 255), 2.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(10.0f, SnakEngine::Color(100, 200, 0, 255), 2.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(20.0f, SnakEngine::Color(0, 255, 100, 255), 2.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);
	ADD_BALL(5.0f, SnakEngine::Color(10, 255, 100, 255), 6.0f, 1.0f, 40.0f, 45.0f, fTotalProbability);


	//random probability for ball spawn
	std::uniform_real_distribution<float> spawnProbability(0.0f, fTotalProbability);
	
	//set size of the internal array to prevent extra allocations
	m_cBalls.reserve(NUM_BALLS);

	CBallSpawn* pBallSpawn = &possibleBalls[0];

	//randomly initialize balls
	for (int i = 0; i < NUM_BALLS; i++)
	{
		//create spawn factor
		float spawnFactor = spawnProbability(randomEngine);

		//find spawnball
		for (size_t j = 0; j < possibleBalls.size(); j++)
		{
			if (spawnFactor <= possibleBalls[j].probability)
			{
				pBallSpawn = &possibleBalls[j];
				break;
			}
		}

		//add ball to the ball vector

		//get ball position
		glm::vec2 pos(randX(randomEngine), randY(randomEngine));

		//get ball direction
		glm::vec2 dir(randDir(randomEngine), randDir(randomEngine));
		if (dir.x != 0.0f || dir.y != 0.0f)
		{
			dir = glm::normalize(dir);
		}
		else
		{
			dir = glm::vec2(1.0f, 0.0f);
		}

		//add ball
		m_cBalls.emplace_back(pBallSpawn->radius, pBallSpawn->mass, pos, dir * pBallSpawn->speed(randomEngine),
			SnakEngine::ResourceManager::GetTexture("Textures/circle.png").ID,
			pBallSpawn->color);

		//add ball to the grid, grid stores ball pointers
		m_pGrid->AddBall(&m_cBalls.back());
	}


}
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

		m_cCamera.Update();
		DrawGame();
	
		m_fFPS = fpsLimiter.End();
	}


}
void CGameWorld::Update(float eclapseTime)
{
	m_cBallController.UpdateBalls(m_cBalls, m_pGrid.get(), eclapseTime, m_iScreenWidth, m_iScreenHeight);
}

void CGameWorld::DrawGame()
{
	//set base depth to 1.0
	glClearDepth(1.0);

	//clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projectMatrix = m_cCamera.GetCameraMatrix();

	m_ballRenderers[m_iCurrentRender]->RenderBalls(m_cSpriteBatch, m_cBalls, projectMatrix);

	//draw HUD
	DrawHud();

	m_cWindow.SwapBuffer();
}


void CGameWorld::DrawHud()
{
	const SnakEngine::Color fontColor(255, 255, 0, 255);
	char buffer[64];
	sprintf(buffer, "%.1f", m_fFPS);

	//InitShaders();

	glActiveTexture(GL_TEXTURE0);

	m_cShader.use();
	GLint textureUniform = m_cShader.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectMatrix = m_cCamera.GetCameraMatrix();
	GLint pUniform = m_cShader.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectMatrix[0][0]);

	m_cSpriteBatch.Begin();

	m_pSpriteFont->draw(m_cSpriteBatch, buffer, glm::vec2(0.0f, m_iScreenHeight - 32.0f), glm::vec2(0.5f, 0.5f), 0.0f, fontColor);

	m_cSpriteBatch.End();
	m_cSpriteBatch.RenderBatch();

	m_cShader.unuse();

}


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
		case SDL_MOUSEMOTION:
			m_cInputManager.SetMouseCoords((float)event.motion.x, (float)event.motion.y);
			m_cBallController.onMouseMove(m_cBalls, (float)event.motion.x, (float)(m_iScreenHeight - event.motion.y));
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_cInputManager.PressKey(event.button.button);
			m_cBallController.onMouseDown(m_cBalls, (float)event.button.x, (float)(m_iScreenHeight - event.button.y));
			break;
		case SDL_MOUSEBUTTONUP:
			m_cInputManager.ReleaseKey(event.button.button);
			m_cBallController.onMouseUp(m_cBalls);
			break;
		default:
			break;
		}
	}

	if (m_cInputManager.isKeyDown(SDLK_ESCAPE))
	{
		m_eGameState = GameState::EXIT;
	}

	if (m_cInputManager.isKeyPressed(SDLK_LEFT))
	{
		m_cBallController.SetGravityDirection(GravityDirection::LEFT);
	}
	else if (m_cInputManager.isKeyPressed(SDLK_RIGHT))
	{
		m_cBallController.SetGravityDirection(GravityDirection::RIGHT);
	}
	else if (m_cInputManager.isKeyPressed(SDLK_UP))
	{
		m_cBallController.SetGravityDirection(GravityDirection::UP);
	}
	else if (m_cInputManager.isKeyPressed(SDLK_DOWN))
	{
		m_cBallController.SetGravityDirection(GravityDirection::DOWN);
	}
	else if (m_cInputManager.isKeyPressed(SDLK_SPACE))
	{
		m_cBallController.SetGravityDirection(GravityDirection::NONE);
	}


	if (m_cInputManager.isKeyPressed(SDLK_1))
	{
		m_iCurrentRender++;
		if (m_iCurrentRender >= m_ballRenderers.size())
		{
			m_iCurrentRender = 0;
		}
	}
}