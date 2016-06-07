#pragma once

#include "Bullet.h"

#include <random>
#include <ctime>

#include <SDL\SDL.h>
#include <glm\gtx\rotate_vector.hpp>


class Gun
{
public:
	Gun(std::string name, float fireSpan, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun();

	void Update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
	
private:

	void Fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

	std::string m_csName;
	
	// frequency by frame
	float m_fFireSpan;
	// bullets at one shot
	int m_iBulletPerShot;
	// accuracy
	float m_iSpread;
	// bullet speed
	float m_fSpeed;
	//bullet damage
	float m_fDamage;
};

