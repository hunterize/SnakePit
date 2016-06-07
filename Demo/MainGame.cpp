#include "MainGame.h"

#include <iostream>
#include <string>


MainGame::MainGame()
{
	m_iScreenWidth = 1024;
	m_iScreenHeight = 768;
	m_eState = GameState::PLAY;
	m_fFPS = 0.0f;
	m_fMaxFPS = 60.0f;
	//m_cPlayerTexture = {};

	//initialize camera orthographic matrix
	m_cCamera.Init(m_iScreenWidth, m_iScreenHeight);
}


MainGame::~MainGame()
{
}


void MainGame::initSystems()
{
	SnakEngine::InitEngine();

	m_cWindow.Create("SnakePit", m_iScreenWidth, m_iScreenHeight, 0);

	initShaders();

	m_cSpriteBatch.Init();

	m_cFPSLimiter.Init(0.0f);

}

void MainGame::initShaders()
{
	m_cColorProgram.CompileShaders("Shaders/ColorShading.vert", "Shaders/ColorShading.frag");
	m_cColorProgram.AddAttribute("vertexPosition");
	m_cColorProgram.AddAttribute("vertexColor");
	m_cColorProgram.AddAttribute("vertexUV");
	m_cColorProgram.LinkShaders();
}

void MainGame::run()
{
	initSystems();


	gameLoop();

}

void MainGame::gameLoop()
{
	while (m_eState != GameState::EXIT)
	{
		m_cFPSLimiter.Begin();

		processInput();

		m_cCamera.Update();

		drawGame();

		m_fFPS = m_cFPSLimiter.End();
		TraceFPS();


	}
}

void MainGame::TraceFPS()
{
	//print only noce every 10 frames
	static int frameCounter = 0;
	frameCounter++;
	if (frameCounter == 10)
	{
		std::cout << m_fFPS << std::endl;
		frameCounter = 0;
	}
}

void MainGame::processInput()
{
	SDL_Event event;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.01f;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_eState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << event.motion.x << " " <<event.motion.y <<std::endl;
			m_cInputManager.SetMouseCoords(event.motion.x, event.motion.y);
			break;

		case SDL_KEYDOWN:
			std::cout << event.key.keysym.sym << std::endl;
			m_cInputManager.PressKey(event.key.keysym.sym);
			break;

		case SDL_KEYUP:
			m_cInputManager.ReleaseKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONUP:
			m_cInputManager.ReleaseKey(event.button.button);
		case SDL_MOUSEBUTTONDOWN:
			m_cInputManager.PressKey(event.button.button);
		}
	}

	if (m_cInputManager.isKeyPressed(SDLK_ESCAPE))
	{
		m_eState = GameState::EXIT;
	}

	if(m_cInputManager.isKeyPressed(SDLK_w))
	{
		m_cCamera.SetPosition(m_cCamera.GetPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}

	if (m_cInputManager.isKeyPressed(SDLK_s))
	{
		m_cCamera.SetPosition(m_cCamera.GetPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (m_cInputManager.isKeyPressed(SDLK_a))
	{
		m_cCamera.SetPosition(m_cCamera.GetPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (m_cInputManager.isKeyPressed(SDLK_d))
	{
		m_cCamera.SetPosition(m_cCamera.GetPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}

	if (m_cInputManager.isKeyPressed(SDLK_q))
	{
		m_cCamera.SetScale(m_cCamera.GetScale() + SCALE_SPEED);
	}

	if (m_cInputManager.isKeyPressed(SDLK_e))
	{
		m_cCamera.SetScale(m_cCamera.GetScale() - SCALE_SPEED);
	}

}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//enable the shader
	m_cColorProgram.use();


	//we are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_cPlayerTexture.ID);

	//get the uniform location
	GLuint textureLocation = m_cColorProgram.GetUniformLocation("mySampler");
	//tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//set the camera matrix
	GLuint cameraLocation = m_cColorProgram.GetUniformLocation("cameraMatrix");
	glm::mat4 cCameraMatrix = m_cCamera.GetCameraMatrix();
	glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, &cCameraMatrix[0][0]);



	m_cSpriteBatch.Begin();

	static SnakEngine::GLTexture texture = SnakEngine::ResourceManager::GetTexture("Textures/PNG/CharacterRight_Standing.png");
	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	SnakEngine::Color color;
	color.a = 255;
	color.r = 255;
	color.g = 255;
	color.b = 255;

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 50; j++)
		{
			m_cSpriteBatch.Draw(position + glm::vec4(0.0f + j * 50.0f, 0.0f + i * 50.0f, 0.0f, 0.0f), uv, texture.ID, 0.0f, color);
		}

	m_cSpriteBatch.End();

	m_cSpriteBatch.RenderBatch();


	glBindTexture(GL_TEXTURE_2D, 0);
	m_cColorProgram.unuse();

	m_cWindow.SwapBuffer();


}
