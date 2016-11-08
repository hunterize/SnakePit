#include "BallController.h"


void CBallController::UpdateBalls(std::vector<CBall>& balls, float elapseTime, int maxX, int maxY)
{
	for (size_t i = 0; i < balls.size(); i++)
	{
		balls[i].m_cPosition += balls[i].m_cVelocity * elapseTime;

		CheckWallCollision(balls[i], maxX, maxY);
	}
}

//events
void CBallController::onMouseDown(std::vector<CBall>& balls, float mouseX, float mouseY)
{}

void CBallController::onMouseUp(std::vector<CBall>& balls)
{}

void CBallController::onMouseMove(std::vector<CBall>& balls, float mouseX, float mouseY)
{}

void CBallController::CheckCollision(CBall& ball1, CBall& ball2)
{

}

void CBallController::CheckWallCollision(CBall& ball, int x, int y)
{
	//collide with wall on x axis
	if (ball.m_cPosition.x < ball.m_fRadius)
	{
		ball.m_cPosition.x = ball.m_fRadius;
		if (ball.m_cVelocity.x < 0)
		{
			ball.m_cVelocity.x *= -1;
		}
	}
	else if (ball.m_cPosition.x + ball.m_fRadius >= x)
	{
		ball.m_cPosition.x = x - ball.m_fRadius - 1;
		if (ball.m_cVelocity.x > 0)
		{
			ball.m_cVelocity.x *= -1;
		}
	}

	//collide with wall on y axis
	if (ball.m_cPosition.y < ball.m_fRadius)
	{
		ball.m_cPosition.y = ball.m_fRadius;
		if (ball.m_cVelocity.y < 0)
		{
			ball.m_cVelocity.y *= -1;
		}
	}
	else if (ball.m_cPosition.y + ball.m_fRadius >= y)
	{
		ball.m_cPosition.y = y - ball.m_fRadius - 1;
		if (ball.m_cVelocity.y > 0)
		{
			ball.m_cVelocity.y *= -1;
		}
	}
}



bool CBallController::IsMouseOnBall(CBall& ball, float mouseX, float mouseY)
{
	return false;
}

glm::vec2 CBallController::GetGravityAccel()
{
	return glm::vec2(0.0f);
}