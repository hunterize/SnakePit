#pragma once
#include "Agent.h"

#include <random>
#include <ctime>

#include <glm\gtx\rotate_vector.hpp>

class Zombie : public Agent 
{
public:
	Zombie();
	virtual ~Zombie();

	void Init(float speed, glm::vec2 pos, float health);
	virtual void Update(const std::vector<std::string>& levelData, float elapseTime) override;

private:
	glm::vec2 m_cDirection;

};

