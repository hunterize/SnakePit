#pragma once

#include <vector>
#include "ball.h"


enum class GravityDirection {NONE, LEFT, RIGHT, UP, DOWN};

class CBallController
{
public:
	void UpdateBalls(std::vector<CBall>& balls, float elapseTime, int maxX, int maxY);

	//events
	void onMouseDown(std::vector<CBall>& balls, float mouseX, float mouseY);
	void onMouseUp(std::vector<CBall>& balls);
	void onMouseMove(std::vector<CBall>& balls, float mouseX, float mouseY);
	
	void SetGravityDirection(GravityDirection dir) { m_gravityDirection = dir; }

private:
	void CheckCollision(CBall& ball1, CBall& ball2);
	bool IsMouseOnBall(CBall& ball, float mouseX, float mouseY);

	glm::vec2 GetGravityAccel();

	int m_iSelectedBall = -1;

	glm::vec2 m_cPreviousPos = glm::vec2(0.0f);
	glm::vec2 m_cGrabOffset = glm::vec2(0.0f);

	GravityDirection m_gravityDirection = GravityDirection::NONE;
};

