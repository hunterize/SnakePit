#include "Camera2D.h"

namespace SnakEngine
{

	Camera2D::Camera2D() : m_cPosition(0, 0),
		m_cCameraMatrix(1.0f),
		m_fScale(1.0f),
		m_bNeedsUpdate(true),
		m_iScreenWidth(500),
		m_iScreenHeight(500),
		m_cOrthoMatrix(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::Init(int screenWidth, int screenHeight)
	{
		m_iScreenWidth = screenWidth;
		m_iScreenHeight = screenHeight;
		
		//this is actually a orthographic projection matrix for 2D rendering
		//use the coordinate system which the origin(0,0) is at bottom left. (windows uses the origin at top left)
		m_cOrthoMatrix = glm::ortho(0.0f, (float)m_iScreenWidth, 0.0f, (float)m_iScreenHeight);
	}

	void Camera2D::Update()
	{
		if (m_bNeedsUpdate)
		{

			//translate
			glm::vec3 translate(-m_cPosition.x + m_iScreenWidth / 2, -m_cPosition.y + m_iScreenHeight / 2, 0.0f);
			m_cCameraMatrix = glm::translate(m_cOrthoMatrix, translate);
			//glm::vec3 translate(m_cPosition.x - m_iScreenWidth / 2, m_cPosition.y - m_iScreenHeight / 2, 0.0f);
			//m_cCameraMatrix = glm::translate(m_cCameraMatrix, translate);

			//scale
			glm::vec3 scale(m_fScale, m_fScale, 0.0f);
			//it's not easy to understand the transformation, but it's pretty like move to center of the sprite and scale it, then move it back to origin point(0.0)
			m_cCameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cCameraMatrix;
			//m_cCameraMatrix = glm::scale(m_cCameraMatrix, scale);
			//m_cCameraMatrix = glm::scale(m_cOrthoMatrix, scale);

			m_bNeedsUpdate = false;

		}
	}

	//convert screen coordinate to the world coordinate
	glm::vec2 Camera2D::ConvertScreenToWorld(glm::vec2 screenCoord)
	{
		screenCoord.y = m_iScreenHeight - screenCoord.y;

		//relative position to the camera position (centre of the screen)
		screenCoord -= glm::vec2(m_iScreenWidth / 2, m_iScreenHeight / 2);

		screenCoord /= m_fScale;

		screenCoord += m_cPosition;
		
		return screenCoord;
	}

	//AABB collision check for the agent and camera to do camera culling
	// position - agent position
	// dimension - agent collision dimension
	bool Camera2D::IsBoxInView(const glm::vec2& position, const glm::vec2& dimension)
	{
		//the actual camera dimension
		glm::vec2 cameraDimension = glm::vec2(m_iScreenWidth, m_iScreenHeight) / (m_fScale);

		const float CLOSEST_X_DISTANCE = dimension.x / 2.0f + cameraDimension.x / 2.0f;
		const float CLOSEST_Y_DISTANCE = dimension.y / 2.0f + cameraDimension.y / 2.0f;

		glm::vec2 agentCentrePosition = position + dimension / 2.0f;

		//distance vector from agent to camera
		glm::vec2 dist = agentCentrePosition - m_cPosition;

		//get depth of the collision
		float xDepth = CLOSEST_X_DISTANCE - abs(dist.x);
		float yDepth = CLOSEST_Y_DISTANCE - abs(dist.y);

		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		return false;
	}
}
