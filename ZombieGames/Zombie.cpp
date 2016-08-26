#include "Zombie.h"



Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::Init(float speed, glm::vec2 pos, float health)
{
	m_cPosition = pos;
	m_fSpeed = speed;
	m_fHealth = health;

	m_cColor.r = 100;
	m_cColor.g = 0;
	m_cColor.b = 255;
	m_cColor.a = 255;

	//create static random engine
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> ranDirection(-1.0f, 1.0f);

	m_cDirection = glm::vec2(ranDirection(randomEngine), ranDirection(randomEngine));

	if (m_cDirection.length() == 0)
	{
		m_cDirection = glm::vec2(1.0f, 0.0f);
	}

	//normalize the direction vector
	m_cDirection = glm::normalize(m_cDirection);

	m_iTextureID = SnakEngine::ResourceManager::GetTexture("Textures/zombie.png").ID;
}

void Zombie::Update(const std::vector<std::string>& levelData, float elapseTime)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> ranRotate(-40.0f, 40.0f);

	m_cPosition += m_cDirection * m_fSpeed * elapseTime;

	//if colliding the level tiles, update the speed direction by randomly rotating it
	if (CollideWithLevel(levelData))
	{
		m_cDirection = glm::rotate(m_cDirection, ranRotate(randomEngine));
	}
}
