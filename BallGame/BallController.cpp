#include "BallController.h"


void CBallController::UpdateBalls(std::vector<CBall>& balls, float elapseTime, int maxX, int maxY)
{}

//events
void CBallController::onMouseDown(std::vector<CBall>& balls, float mouseX, float mouseY)
{}

void CBallController::onMouseUp(std::vector<CBall>& balls)
{}

void CBallController::onMouseMove(std::vector<CBall>& balls, float mouseX, float mouseY)
{}

void CBallController::CheckCollision(CBall& ball1, CBall& ball2)
{}
bool CBallController::IsMouseOnBall(CBall& ball, float mouseX, float mouseY)
{
	return false;
}

glm::vec2 CBallController::GetGravityAccel()
{
	return glm::vec2(0.0f);
}