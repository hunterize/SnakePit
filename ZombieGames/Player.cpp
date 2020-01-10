#include "Player.h"



Player::Player()
{
	m_iCurrentGun = -1;
}


Player::~Player()
{
}

void Player::Init(float speed, glm::vec2 pos, SnakEngine::InputManager* inputManager, SnakEngine::Camera2D* camera, std::vector<Bullet>* bullets, float health)
{
	m_fSpeed = speed;
	m_cPosition = pos;
	m_pInputManager = inputManager;
	m_pCamera = camera;
	m_pBullets = bullets;
	m_fHealth = health;

	m_cColor.r = 255;
	m_cColor.g = 255;
	m_cColor.b = 255;
	m_cColor.a = 255;

	m_iTextureID = SnakEngine::ResourceManager::GetTexture("Textures/player.png").ID;
}

void Player::Update(const std::vector<std::string>& levelData, float elapseTime)
{
	//move the player
	if (m_pInputManager->isKeyDown(SDLK_w))
	{
		m_cPosition.y += m_fSpeed * elapseTime;
	}
	if (m_pInputManager->isKeyDown(SDLK_s))
	{
		m_cPosition.y -= m_fSpeed * elapseTime;
	}
	if (m_pInputManager->isKeyDown(SDLK_a))
	{
		m_cPosition.x -= m_fSpeed * elapseTime;
	}
	if (m_pInputManager->isKeyDown(SDLK_d))
	{
		m_cPosition.x += m_fSpeed * elapseTime;
	}

	//select weapon
	if (m_pInputManager->isKeyDown(SDLK_1) && m_cGuns.size() >= 0)
	{
		m_iCurrentGun = 0;
	}
	else if (m_pInputManager->isKeyDown(SDLK_2) && m_cGuns.size() >= 1)
	{
		m_iCurrentGun = 1;
	}
	else if (m_pInputManager->isKeyDown(SDLK_3) && m_cGuns.size() >= 2)
	{
		m_iCurrentGun = 2;
	}

	glm::vec2 mouseCoords = m_pInputManager->GetMouseCoords();
	mouseCoords = m_pCamera->ConvertScreenToWorld(mouseCoords);

	//get bullet position and direction
	glm::vec2 bulletPos = m_cPosition + glm::vec2(AGENT_RADIUS);
	glm::vec2 bulletDir = glm::normalize(mouseCoords - bulletPos);

	//update player direction to point to the mouse
	m_cDirection = bulletDir;

	if (m_iCurrentGun != -1)
	{

		//let the bullet start from the outside of the player
		bulletPos -= glm::vec2(BULLET_RADIUS);
		bulletPos += glm::vec2(AGENT_RADIUS) * bulletDir;

		m_cGuns[m_iCurrentGun]->Update(m_pInputManager->isKeyDown(SDL_BUTTON_LEFT), bulletPos, bulletDir, *m_pBullets);

	}

	//collide with level
	CollideWithLevel(levelData);

}

void Player::AddGun(Gun* gun)
{
	m_cGuns.push_back(gun);

	if (m_iCurrentGun == -1)
	{
		m_iCurrentGun = 0;
	}
}