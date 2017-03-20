
#include "GamePlayScreen.h"

#include <SnakEngine\IGameWorld.h>
#include <SDL\SDL.h>
#include <random>

CGamePlayScreen::CGamePlayScreen(SnakEngine::SWindow* pWindow) : m_cWindow(pWindow)
{}

CGamePlayScreen::~CGamePlayScreen()
{}

int CGamePlayScreen::GetNextScreenIndex() const
{
	return -1;
}

int CGamePlayScreen::GetPreviousScreenIndex() const
{
	return -1;
}

void CGamePlayScreen::CGamePlayScreen::Create()
{

	SDL_SetWindowTitle(m_cWindow->GetWindow(), "NinjaGame");
	//SDL_SetWindowSize(m_cWindow->GetWindow(), 1600, 900);

	b2Vec2 gravity(0.0f, -9.8f);
	m_pWorld = std::make_unique<b2World>(gravity);

	//create playground 
	b2BodyDef groundDef;
	groundDef.position.Set(0.0f, -40.0f);
	b2Body* pGroundBody = m_pWorld->CreateBody(&groundDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox(200.0f, 20.f);
	pGroundBody->CreateFixture(&groundShape, 0.0f);

	
	//generate random engine
	std::mt19937 randEngine;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(10.0f, 50.0f);
	std::uniform_real_distribution<float> size(0.5f, 4.0f);
	std::uniform_int_distribution<int> color(50, 255);

	//create crates
	for (int i = 0; i < 100; i++)
	{
		SnakEngine::Color col;
		col.r = color(randEngine);
		col.g = color(randEngine);
		col.b = color(randEngine);
		col.a = 255;
		CCrate crate;
		crate.Init(m_pWorld.get(), glm::vec2(xPos(randEngine), yPos(randEngine)), glm::vec2(size(randEngine), size(randEngine)), col);
		m_cCrates.push_back(crate);
	}

	//initialize engine system
	m_cSpriteBatch.Init();

	m_cShader.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_cShader.AddAttribute("vertexPosition");
	m_cShader.AddAttribute("vertexColor");
	m_cShader.AddAttribute("vertexUV");
	m_cShader.LinkShaders();

	m_cTexture = SnakEngine::ResourceManager::GetTexture("Textures/bricks_top.png");

	m_cCamera.Init(m_cWindow->GetWidth(), m_cWindow->GetHeight());
	m_cCamera.SetScale(10.0f);
}

void CGamePlayScreen::CGamePlayScreen::Destroy()
{}

void CGamePlayScreen::OnStart()
{
}

void CGamePlayScreen::OnExit()
{}

void CGamePlayScreen::Update()
{
	m_cCamera.Update();
	UpdateInput();

	m_pWorld->Step(1.0f / 60.0f, 6, 2);
}

void CGamePlayScreen::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_cShader.use();

	GLint textureUniform = m_cShader.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = m_cCamera.GetCameraMatrix();
	GLint pUniform = m_cShader.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_cSpriteBatch.Begin();

	for (auto& box : m_cCrates)
	{
		glm::vec4 destRect;
		destRect.x = box.GetBody()->GetPosition().x - box.GetDimension().x / 2.0f;
		destRect.y = box.GetBody()->GetPosition().y - box.GetDimension().y / 2.0f;
		destRect.z = box.GetDimension().x;
		destRect.w = box.GetDimension().y;

		m_cSpriteBatch.Draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_cTexture.ID, 0.0f, box.GetColor(), box.GetBody()->GetAngle());

	}

	m_cSpriteBatch.End();
	m_cSpriteBatch.RenderBatch();

	m_cShader.unuse();
}

void CGamePlayScreen::UpdateInput()
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt))
	{
		m_pGame->OnSDLEvent(evt);
	}
}