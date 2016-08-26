#include "Human.h"



Human::Human():m_iFrame(0)
{
}


Human::~Human()
{
}

void Human::Init(float speed, glm::vec2 pos, float health)
{
	m_fSpeed = speed;
	m_cPosition = pos;
	m_fHealth = health;

	m_cColor.r = 0;
	m_cColor.g = 255;
	m_cColor.b = 0;
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

	m_iTextureID = SnakEngine::ResourceManager::GetTexture("Textures/human.png").ID;


}

void Human::Update(const std::vector<std::string>& levelData, float elapseTime)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> ranRotate(-40.0f, 40.0f);

	m_cPosition += m_cDirection * m_fSpeed * elapseTime;


	//update speed direction by randomly rotating it every 200 frames
	if (m_iFrame == 1000)
	{
		m_cDirection = glm::rotate(m_cDirection, ranRotate(randomEngine));
		m_iFrame = 0;
	}
	else
	{
		m_iFrame++;
	}


	//if colliding the level tiles, update the speed direction by randomly rotating it
	if (CollideWithLevel(levelData))
	{
		m_cDirection = glm::rotate(m_cDirection, ranRotate(randomEngine));
	}
}
