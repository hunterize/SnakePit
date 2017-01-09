#include "BallController.h"


void CBallController::UpdateBalls(std::vector<CBall>& balls, CGrid* pGrid, float elapseTime, int maxX, int maxY)
{
	glm::vec2 gravity = GetGravityAccel();

	for (size_t i = 0; i < balls.size(); i++)
	{
		if (i != m_iSelectedBall)
		{
			//udpate ball position
			balls[i].m_cPosition += balls[i].m_cVelocity * elapseTime;

			//apply friction
			const float FRICTION = 3.0f;

			if(glm::length(balls[i].m_cVelocity) < 0.01f)
			{
				balls[i].m_cVelocity = glm::vec2(0.0f);
			}
			else
			{
				balls[i].m_cVelocity -= elapseTime * glm::normalize(balls[i].m_cVelocity) * FRICTION;
			}

			//apply gravity
			balls[i].m_cVelocity += gravity * elapseTime;

		}

		//check collision with wall
		CheckWallCollision(balls[i], maxX, maxY);


		CCell* pNewCell = pGrid->GetCell(balls[i].m_cPosition);
		if (pNewCell != balls[i].m_pCell)
		{
			pGrid->RemoveBall(&balls[i]);
			pGrid->AddBall(&balls[i], pNewCell);
		}


		//check collision with balls
		//for (int j = i + 1; j < balls.size(); j++)
		//{
		//	CheckCollision(balls[i], balls[j]);
		//}

	}

	CheckCollision(pGrid);


	//update velocity of the selected ball when being grabbed
	if (m_iSelectedBall != -1)
	{
		m_cPreviousVel = balls[m_iSelectedBall].m_cVelocity;
		balls[m_iSelectedBall].m_cVelocity = ((balls[m_iSelectedBall].m_cPosition - m_cPreviousPos) * 2.0f / elapseTime - m_cPreviousVel) * 0.1f;
		//balls[m_iSelectedBall].m_cVelocity = balls[m_iSelectedBall].m_cPosition - m_cPreviousPos;
		m_cPreviousPos = balls[m_iSelectedBall].m_cPosition;
		
	}
}

//events
void CBallController::onMouseDown(std::vector<CBall>& balls, float mouseX, float mouseY)
{
	for (int i = 0; i < balls.size(); i++)
	{
		if (IsMouseOnBall(balls[i], mouseX, mouseY))
		{
			m_iSelectedBall = i;
			m_cGrabOffset = glm::vec2(mouseX, mouseY) - balls[i].m_cPosition;
			m_cPreviousPos = balls[i].m_cPosition;
			m_cPreviousVel = balls[i].m_cVelocity;
			m_uiPreviousTime = SDL_GetTicks();
			balls[i].m_cVelocity = glm::vec2(0.0f);
			return;
		}
	}
}


//update the velocity of the selected ball when mouse key is released
void CBallController::onMouseUp(std::vector<CBall>& balls)
{
	if (m_iSelectedBall != -1)
	{
		float timeSpan = (SDL_GetTicks() - m_uiPreviousTime ) / 1000.0f;
		balls[m_iSelectedBall].m_cVelocity = (balls[m_iSelectedBall].m_cPosition - m_cPreviousPos) * 2.0f / timeSpan - m_cPreviousVel;

		m_iSelectedBall = -1;
	}
}

void CBallController::onMouseMove(std::vector<CBall>& balls, float mouseX, float mouseY)
{
	if (m_iSelectedBall != -1)
	{
		balls[m_iSelectedBall].m_cPosition = glm::vec2(mouseX, mouseY) - m_cGrabOffset;
	}
}


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
		//ball1.m_cPosition -= distanceDir * collisionDepth * (ball2.m_fMass / ball1.m_fMass) / 2.0f;
		//ball2.m_cPosition += distanceDir * collisionDepth * (ball1.m_fMass / ball2.m_fMass) / 2.0f;

		ball1.m_cPosition -= distanceDir * collisionDepth * (ball1.m_fRadius / (ball1.m_fRadius + ball2.m_fRadius));
		ball2.m_cPosition += distanceDir * collisionDepth * (ball2.m_fRadius / (ball1.m_fRadius + ball2.m_fRadius));


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


		//add more effect to the collision, the faster ball transfers color to the other
		if (glm::length(ball1.m_cVelocity + ball2.m_cVelocity) > 0.5f)
		{
			glm::length(ball1.m_cVelocity) < glm::length(ball2.m_cVelocity) ? ball2.m_cColor = ball1.m_cColor : ball1.m_cColor = ball2.m_cColor;
		}

	}
}

void CBallController::CheckCollision(CBall* pBall, std::vector<CBall*>& balls, int iStartIndex)
{
	for (int i = iStartIndex; i < balls.size(); i++)
	{
		CheckCollision(*pBall, *balls[i]);
	}
}

void CBallController::CheckCollision(CGrid* pGrid)
{
	for (int i = 0; i < pGrid->m_cells.size(); i++)
	{
		int column = i % pGrid->m_iCellX;
		int row = i / pGrid->m_iCellX;

		for (int j = 0; j < pGrid->m_cells[i].m_balls.size(); j++)
		{
			//check collision with own cell
			CheckCollision(pGrid->m_cells[i].m_balls[j], pGrid->m_cells[i].m_balls, j + 1);

			if (column > 0)
			{
				//check left cell
				CheckCollision(pGrid->m_cells[i].m_balls[j], pGrid->GetCell(column - 1, row)->m_balls, 0);
				if (row > 0)
				{
					//check top left cell
					CheckCollision(pGrid->m_cells[i].m_balls[j], pGrid->GetCell(column - 1, row - 1)->m_balls, 0);
				}
				if (row < pGrid->m_iCellY - 1)
				{
					//check bottom left cell
					CheckCollision(pGrid->m_cells[i].m_balls[j], pGrid->GetCell(column, row + 1)->m_balls, 0);
				}
			}

			if (row > 0)
			{
				//check top cell
				CheckCollision(pGrid->m_cells[i].m_balls[j], pGrid->GetCell(column, row - 1)->m_balls, 0);
			}

		}
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



//check if mouse position is on the ball
bool CBallController::IsMouseOnBall(CBall& ball, float mouseX, float mouseY)
{
	return (mouseX >= ball.m_cPosition.x - ball.m_fRadius && mouseX <= ball.m_cPosition.x + ball.m_fRadius
		    && mouseY >= ball.m_cPosition.y - ball.m_fRadius && mouseY <= ball.m_cPosition.y + ball.m_fRadius);
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