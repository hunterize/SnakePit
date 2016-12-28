#pragma once

#include <vector>
#include "ball.h"
#include <SDL\SDL.h>
#include "Grid.h"


enum class GravityDirection {NONE, LEFT, RIGHT, UP, DOWN};

class CBallController
{
public:
	void UpdateBalls(std::vector<CBall>& balls, CGrid* pGrid, float elapseTime, int maxX, int maxY);

	//events
	void onMouseDown(std::vector<CBall>& balls, float mouseX, float mouseY);
	void onMouseUp(std::vector<CBall>& balls);
	void onMouseMove(std::vector<CBall>& balls, float mouseX, float mouseY);
	
	void SetGravityDirection(GravityDirection dir) { m_gravityDirection = dir; }

private:
	//check ball to ball collision
	void CheckCollision(CBall& ball1, CBall& ball2);
	//check ball to cell collision
	void CheckCollision(CBall* pBall, std::vector<CBall*>& balls, int iStartIndex);
	//check ball to grid collision
	void CheckCollision(CGrid* pGrid);

	//check wall collision
	void CheckWallCollision(CBall& ball, int x, int y);

	bool IsMouseOnBall(CBall& ball, float mouseX, float mouseY);

	glm::vec2 GetGravityAccel();

	int m_iSelectedBall = -1;

	glm::vec2 m_cPreviousPos = glm::vec2(0.0f);
	glm::vec2 m_cPreviousVel = glm::vec2(0.0f);
	glm::vec2 m_cGrabOffset = glm::vec2(0.0f);
	Uint32 m_uiPreviousTime = 0.0f;

	GravityDirection m_gravityDirection = GravityDirection::NONE;
};

