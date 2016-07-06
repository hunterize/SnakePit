#include "GameWorld.h"


namespace
{
	const float PLAYER_SPEED = 350.0f;
	const float HUMAN_SPEED = 350.0f;
	const float ZOMBIE_SPEED = 250.0f;
	const float SCALE_SPEED = 0.2f;
	const float BULLET_SPEED = 1250.0f;
}

GameWorld::GameWorld() : 
	m_iScreenWidth(1024),
	m_iScreenHeight(768),
	m_eGameState(GameState::PLAY), 
	m_fFPS(1.0f), 
	m_iCurrentLevel(0), 
	m_pPlayer(nullptr),
	m_iHumansKilled(0),
	m_iZombiesKilled(0),
	m_iHumansInfected(0)
{
}


GameWorld::~GameWorld()
{
	for (int i = 0; i < m_cLevels.size(); i++)
	{
		delete m_cLevels[i];
	}

	for each (Human* p in m_cHumens)
	{
		delete p;
	}
}


void GameWorld::Run()
{
	InitSystem();

	InitLevel();

	GameLoop();
}

void GameWorld::InitSystem()
{
	SnakEngine::InitEngine();
	m_cWindow.Create("ZombieGame", m_iScreenWidth, m_iScreenHeight, 0);
	
	//grey background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitShaders();

	m_cSpriteBatch.Init();
	m_cHudSpriteBatch.Init();

	//initialize sprite font
	m_pSpriteFont = new SnakEngine::SpriteFont("Fonts/chintzy.ttf", 64);

	m_cCamera.Init(m_iScreenWidth, m_iScreenHeight);
	m_cHudCamera.Init(m_iScreenWidth, m_iScreenHeight);
	m_cHudCamera.SetPosition(glm::vec2(m_iScreenWidth / 2, m_iScreenHeight / 2));


}

void GameWorld::InitLevel()
{
	m_cLevels.push_back(new Level("Levels/level1.txt"));

	//initialize player, add it to the human vector
	m_pPlayer = new Player();
	m_pPlayer->Init(PLAYER_SPEED, m_cLevels[m_iCurrentLevel]->GetPlayerPos(), &m_cInputManager, &m_cCamera, &m_cBullet, 100.0f);
	//the first element of human vector is player
	m_cHumens.push_back(m_pPlayer);

	m_pPlayer->AddGun(new Gun("Magnum", 800, 1, 0.0f, 200.0f, BULLET_SPEED * 1.5f));
	m_pPlayer->AddGun(new Gun("Shotgun", 500, 6, 0.6f, 100.0f, BULLET_SPEED * 2.0f));
	m_pPlayer->AddGun(new Gun("MP5", 200, 1, 0.1f, 70.0f, BULLET_SPEED * 3.0f));


	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	//get random position of x and y in the level vector
	std::uniform_int_distribution<int> randX(2, m_cLevels[m_iCurrentLevel]->GetColumn() - 2);
	std::uniform_int_distribution<int> randY(2, m_cLevels[m_iCurrentLevel]->GetRow() - 2);

	//add random humans 
	for (int i = 0; i < m_cLevels[m_iCurrentLevel]->GetHumanNumber(); i++)
	{
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine)*TILE_HEIGHT);
		Human* tmp = new Human();
		tmp->Init(HUMAN_SPEED, pos, 100.0f);
		m_cHumens.push_back(tmp);
		
		//std::cout << tmp->GetPosition().x << " " << tmp->GetPosition().y << std::endl;
	}

	//add zombies 
	for (int i = 0; i < m_cLevels[m_iCurrentLevel]->GetZombiePosition().size(); i++)
	{
		m_cZombies.push_back(new Zombie);
		m_cZombies.back()->Init(ZOMBIE_SPEED, m_cLevels[m_iCurrentLevel]->GetZombiePosition()[i], 200.0f);
	}


}

void GameWorld::InitShaders()
{
	m_cShader.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_cShader.AddAttribute("vertexPosition");
	m_cShader.AddAttribute("vertexColor");
	m_cShader.AddAttribute("vertexUV");
	m_cShader.LinkShaders();

}

void GameWorld::GameLoop()
{
	SnakEngine::FpsLimiter fpsLimiter;
	fpsLimiter.SetTargetFPS(0.0f);
	float timeSpan = 0.0f;
	//delta time for the maxine value of physic time
	float dt = 1 / 10.0;

	while (m_eGameState == GameState::PLAY)
	{

		fpsLimiter.Begin();
		timeSpan = fpsLimiter.GetFrameTime() / 1000.0f;

		IsGameOver();

		ProcessInput();

		while (timeSpan > 0.0)
		{
			float deltaTime = std::min(timeSpan, dt);


			UpdateAgent(deltaTime);

			UpdateBullet(deltaTime);

			UpdateCamera(deltaTime);

			timeSpan -= deltaTime;
		}

		//UpdateAgent(timeSpan);

		//UpdateBullet(timeSpan);

		//UpdateCamera(timeSpan);

		DrawGame();

		m_fFPS = fpsLimiter.End();
		//TraceFPS();
	}

	if (m_eGameState == GameState::WIN)
	{
		std::cout << "**** YOU WIN ***" << std::endl;
	}
	else if (m_eGameState == GameState::LOSE)
	{
		std::cout << "*** YOU LOST ***" << std::endl;
	}

	CheckGameStatus();

	SnakEngine::fatalError("Game Over");
}

void GameWorld::TraceFPS()
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

void GameWorld::ProcessInput()
{
	SDL_Event evnt;

	m_cInputManager.Update();

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			m_eGameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_cInputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_cInputManager.PressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_cInputManager.ReleaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_cInputManager.PressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_cInputManager.ReleaseKey(evnt.button.button);
			break;
		}
	}

	if(m_cInputManager.isKeyDown(SDLK_ESCAPE))
	{
		m_eGameState = GameState::EXIT;
	}

}

void GameWorld::UpdateCamera(float elapseTime)
{
	if (m_cInputManager.isKeyDown(SDLK_e))
	{
		m_cCamera.SetScale(m_cCamera.GetScale() + SCALE_SPEED * elapseTime);
		//m_cHudCamera.SetScale(m_cHudCamera.GetScale() + SCALE_SPEED * elapseTime);
	}
	if (m_cInputManager.isKeyDown(SDLK_q))
	{
		m_cCamera.SetScale(m_cCamera.GetScale() - SCALE_SPEED * elapseTime);
		//m_cHudCamera.SetScale(m_cHudCamera.GetScale() - SCALE_SPEED * elapseTime);
	}

	m_cCamera.SetPosition(m_pPlayer->GetPosition());

	/*
	glm::vec2 pos = m_pPlayer->GetPosition();

	if (m_iScreenWidth / 2 < pos.x && pos.x < (m_cLevels[m_iCurrentLevel]->GetColumn() * TILE_WIDTH - m_iScreenWidth / 2)
		&& m_iScreenHeight / 2 < pos.y && pos.y < (m_cLevels[m_iCurrentLevel]->GetRow() * TILE_HEIGHT - m_iScreenHeight / 2))
	{
		std::cout << "set camera position" << std::endl;
		//m_cCamera.SetPosition(m_pPlayer->GetPosition());

	}
	else
	{
		std::cout << "NOT set camera position" << std::endl;
	}
	*/

	m_cCamera.Update();

	m_cHudCamera.Update();

}

void GameWorld::UpdateAgent(float elapseTime)
{
	///****update agents including humans and zombies****
	//update all humans including player
	for each (Human* p in m_cHumens)
	{
		p->Update(m_cLevels[m_iCurrentLevel]->GetLevelData(),elapseTime);
	}

	//update Zombies
	for each(Zombie* z in m_cZombies)
	{
		z->Update(m_cLevels[m_iCurrentLevel]->GetLevelData(), elapseTime);
	}

	///****do collision****
	//do human collision
	for (int i = 0; i < m_cHumens.size(); i++)
	{
		for (int j = i + 1; j < m_cHumens.size(); j++)
		{
			m_cHumens[i]->CollideWithAgent(m_cHumens[j]);
		}
	}

	//do Zombie collision
	for (int i = 0; i < m_cZombies.size(); i++)
	{
		//do zombie collision with other zombies
		for (int j = i + 1; j < m_cZombies.size(); j++)
		{
			m_cZombies[i]->CollideWithAgent(m_cZombies[j]);
		}

		
		//do zombie collision with humans, human[0] is player	
		std::vector<Human*>::iterator it;
		for (it = m_cHumens.begin() + 1; it != m_cHumens.end();)
		{
			//zombie bites human
			if (m_cZombies[i]->CollideWithAgent(*it))
			{
				//turns human to zombie
				m_cZombies.push_back(new Zombie);
				m_cZombies.back()->Init(ZOMBIE_SPEED, (*it)->GetPosition(), 200.0f);
				
				//delete human
				delete *it;
				it = m_cHumens.erase(it);
				m_iHumansInfected++;
			}
			else
			{
				++it;
			}
		}
		
		//do collision with player
		if (m_cZombies[i]->CollideWithAgent(m_pPlayer))
		{
			if(m_pPlayer->ApplyDemage(10.0f))
			{
				m_eGameState = GameState::LOSE;
			}
		}
	}
	


}

void GameWorld::UpdateBullet(float elapseTime)
{
	//do collision with levels
	for (int i = 0; i < m_cBullet.size();)
	{
		if(m_cBullet[i].Update(m_cLevels[m_iCurrentLevel]->GetLevelData(), elapseTime))
		{
			m_cBullet[i] = m_cBullet.back();
			m_cBullet.pop_back();
		}
		else
		{
			i++;
		}
	}


	bool isRemoved = false;
	// do collision with humans and zombies
	for (int i = 0; i < m_cBullet.size(); i++)
	{
		isRemoved = false;
		//do collision with zombies
		for (int j = 0; j < m_cZombies.size();)
		{
			if (m_cBullet[i].CollideWithAgent(m_cZombies[j]))
			{
				//do damage to zombile
				if (m_cZombies[j]->ApplyDemage(m_cBullet[i].GetDamage()))
				{
					//if zombile health <= 0, remove it from the list
					delete m_cZombies[j];
					m_cZombies[j] = m_cZombies.back();
					m_cZombies.pop_back();
					m_iZombiesKilled++;
				}
				else
				{
					j++;
				}

				//if bullet collides with zombile, remove it from the list
				m_cBullet[i] = m_cBullet.back();
				m_cBullet.pop_back();
				isRemoved = true;
				//move back the iterator of the bullet vector to make sure not to skip a bullet
				i--;

				break;
			}
			else
			{
				j++;
			}
		}

		//if bullet is removed (collide with zombies), skip the collision with humans 
		if (!isRemoved)
		{
			//do collision with human
			//skip first element - player
			for (int j = 1; j < m_cHumens.size();)
			{
				if (m_cBullet[i].CollideWithAgent(m_cHumens[j]))
				{
					//do damage to zombile
					if (m_cHumens[j]->ApplyDemage(m_cBullet[i].GetDamage()))
					{
						//if human health <= 0, remove it from the list
						delete m_cHumens[j];
						m_cHumens[j] = m_cHumens.back();
						m_cHumens.pop_back();
						m_iHumansKilled++;
					}
					else
					{
						j++;
					}

					//if bullet collides with zombile, remove it from the list
					m_cBullet[i] = m_cBullet.back();
					m_cBullet.pop_back();

					//move back the iterator of the bullet vector to make sure not to skip a bullet
					i--;

					break;
				}
				else
				{
					j++;
				}
			}
		}

	}

}


void GameWorld::CheckGameStatus()
{
	std::cout << m_pPlayer->GetHealth() << " health" << std::endl;
	std::cout << m_cLevels[m_iCurrentLevel]->GetHumanNumber() << " humans are spawned" << std::endl;
	std::cout << m_cHumens.size() - 1 << " humans are alive" << std::endl;
	std::cout << m_iHumansInfected << " humans are infected" << std::endl;
	std::cout << m_cZombies.size() << " zombies are alive" << std::endl;
	std::cout << m_iHumansKilled << " humans are killed" << std::endl;
	std::cout << m_iZombiesKilled << " zombies are killed" << std::endl;

}

bool GameWorld::IsGameOver()
{
	if (m_pPlayer->GetHealth() < 0.0f)
	{
		m_eGameState = GameState::LOSE;
	}
	//else if (m_cHumens.size() == 1)
	//{
	//	m_eGameState = GameState::LOSE;
	//}
	else if (m_cZombies.empty())
	{
		m_eGameState = GameState::WIN;
	}

	return false;
}

void GameWorld::DrawGame()
{
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_cShader.use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_cShader.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	//Get projection matrix
	glm::mat4 projectionMatrix = m_cCamera.GetCameraMatrix();
	GLint pUniform = m_cShader.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw Tiles
	m_cLevels[m_iCurrentLevel]->draw();

	//begin to draw agents and bullets
	m_cSpriteBatch.Begin();

	

	for each (Human* p in m_cHumens)
	{
		if (m_cCamera.IsBoxInView(p->GetPosition(), glm::vec2(p->GetRadius() * 2.0f)))
		{
			p->Draw(m_cSpriteBatch);
		}
	}

	for each(Zombie* z in m_cZombies)
	{
		if (m_cCamera.IsBoxInView(z->GetPosition(), glm::vec2(z->GetRadius() * 2.0f)))
		{
			z->Draw(m_cSpriteBatch);
		}
	}

	for each(Bullet b in m_cBullet)
	{
		b.Draw(m_cSpriteBatch);
	}

	//end to draw agents
	m_cSpriteBatch.End();

	m_cSpriteBatch.RenderBatch();

	//draw HUD
	DrawHud();

	m_cShader.unuse();

	m_cWindow.SwapBuffer();

}

void GameWorld::DrawHud()
{
	char buffer[256];

	glm::mat4 projectionMatrix = m_cHudCamera.GetCameraMatrix();
	GLint pUniform = m_cShader.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_cHudSpriteBatch.Begin();

	sprintf_s(buffer, "Humans are alive : %d", m_cHumens.size() - 1);
	m_pSpriteFont->draw(m_cHudSpriteBatch, buffer, glm::vec2(0, 0), glm::vec2(0.4),0.0f, SnakEngine::Color(255, 255, 255, 255));


	sprintf_s(buffer, "FPS : %0.1f", m_fFPS);
	m_pSpriteFont->draw(m_cHudSpriteBatch, buffer, glm::vec2(0, m_iScreenHeight - 26), glm::vec2(0.4), 0.0f, SnakEngine::Color(255, 255, 255, 255));


	m_cHudSpriteBatch.End();
	m_cHudSpriteBatch.RenderBatch();
}


