#include "Gun.h"



Gun::Gun(std::string name, float fireSpan, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed):
	m_csName(name), m_fFireSpan(fireSpan), m_iBulletPerShot(bulletsPerShot), m_iSpread(spread), m_fDamage(bulletDamage), m_fSpeed(bulletSpeed)
{

}


Gun::~Gun()
{
}

void Gun::Update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	static float previousTime = 0;
	float currentTime = SDL_GetTicks();

	if (currentTime - previousTime >= m_fFireSpan && isMouseDown)
	{
		Fire(direction, position, bullets);
		previousTime = currentTime;
	}
}


//private function
void Gun::Fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets)
{
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_iSpread, m_iSpread);

	for (int i = 0; i < m_iBulletPerShot; i++)
	{
		//add new bullet, rotate the bullet direction according to the spread
		bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine)),m_fDamage, m_fSpeed);
	}
}