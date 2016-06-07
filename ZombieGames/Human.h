#pragma once

#include <random>
#include <ctime>	

#include <glm\gtx\rotate_vector.hpp>

#include "Agent.h"



class Human : public Agent
{
public:
	Human();
	virtual ~Human();
	void Init(float speed, glm::vec2 pos, float health);
	virtual void Update(const std::vector<std::string>& levelData, float elapseTime) override;

private:
	glm::vec2 m_cDirection;
	int m_iFrame;
};

