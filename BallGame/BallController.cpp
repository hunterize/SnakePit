#include "BallController.h"


void CBallController::UpdateBalls(std::vector<CBall>& balls, float elapseTime, int maxX, int maxY)
{
	glm::vec2 gravity = GetGravityAccel();

	for (size_t i = 0; i < balls.size(); i++)
	{
		//udpate ball position
		balls[i].m_cPosition += balls[i].m_cVelocity * elapseTime;

		//check collision with wall
		CheckWallCollision(balls[i], maxX, maxY);


		//check collision with balls
		for (int j = i + 1; j < balls.size(); j++)
		{
			CheckCollision(balls[i], balls[j]);
		}

		//apply gravity
		balls[i].m_cVelocity += gravity * elapseTime;


	}
}

//events
void CBallController::onMouseDown(std::vector<CBall>& balls, float mouseX, float mouseY)
{}

void CBallController::onMouseUp(std::vector<CBall>& balls)
{}

void CBallController::onMouseMove(std::vector<CBall>& balls, float mouseX, float mouseY)
{}


//do ball collision by elastic collision
void CBallController::CheckCollision(CBall& ball1, CBall& ball2)
{
	glm::vec2 distanceVec = ball2.m_cPosition - ball1.m_cPosition;
	glm::vec2 distanceDir = glm::normalize(distanceVec);

	float distance = glm::length(distanceVec);
	float collisionDistance = ball1.m_fRadius + ball2.m_fRadius;

	float collisionDepth = collisionDistance - distance;

	//elastic collision
	if (collisionDepth > 0 && distance > 0)
	{
		//do collision
		ball1.m_cPosition -= distanceDir * collisionDepth * (ball2.m_fMass / ball1.m_fMass) / 2.0f;
		ball2.m_cPosition += distanceDir * collisionDepth * (ball1.m_fMass / ball2.m_fMass) / 2.0f;

		//velocity scalor projection along the collision direction
		float ball1Vpv = glm::dot(ball1.m_cVelocity, distanceDir);
		float ball2Vpv = glm::dot(ball2.m_cVelocity, distanceDir);

		//ball velocity vector along the collision direction before collision
		glm::vec2 ball1Vp = ball1Vpv * distanceDir;
		glm::vec2 ball2Vp = ball2Vpv * distanceDir;

		//1D elastic collision equation, velocity value after collision
		float ball1Vpv1 = (ball1Vpv * (ball1.m_fMass - ball2.m_fMass) + 2 * ball2.m_fMass * ball2Vpv) / (ball1.m_fMass + ball2.m_fMass);
		float ball2Vpv1 = (ball2Vpv * (ball2.m_fMass - ball1.m_fMass) + 2 * ball1.m_fMass * ball1Vpv) / (ball1.m_fMass + ball2.m_fMass);

		//ball velocity vector along the collision direction after collision
		glm::vec2 ball1Vp1 = ball1Vpv1 * distanceDir;
		glm::vec2 ball2Vp1 = ball2Vpv1 * distanceDir;

		ball1.m_cVelocity += ball1Vp1 - ball1Vp;
		ball2.m_cVelocity += ball2Vp1 - ball2Vp;

	}
}

//check collision with the wall
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
	const float GRAVITY = 10.0f;
	glm::vec2 gravity;

	switch (m_gravityDirection)
	{
	case GravityDirection::DOWN :
	{
		gravity = glm::vec2(0.0f, -GRAVITY);
		break;
	}
	case GravityDirection::UP:
	{
		gravity = glm::vec2(0.0f, GRAVITY);
		break;
	}
	case GravityDirection::LEFT:
	{
		gravity = glm::vec2(-GRAVITY, 0.0f);
		break;
	}
	case GravityDirection::RIGHT:
	{
		gravity = glm::vec2(GRAVITY, 0.0f);
		break;
	}
	default:
		gravity = glm::vec2(0.0f);
		break;
	}

	return gravity;
}