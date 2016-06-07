#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


namespace SnakEngine
{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void Init(int screenWidth, int screenHeight);
		void Update();
		glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoord);


		//settters
		void SetPosition(const glm::vec2& newPosition)
		{
			m_cPosition = newPosition;
			m_bNeedsUpdate = true;
		}

		void SetScale(float newScale)
		{
			m_fScale = newScale;
			m_bNeedsUpdate = true;
		}


		//getters
		glm::vec2 GetPosition()
		{
			return m_cPosition;
		}

		float GetScale()
		{
			return m_fScale;
		}

		glm::mat4 GetCameraMatrix()
		{
			return m_cCameraMatrix;
		}

	private:
		bool m_bNeedsUpdate;
		float m_fScale;
		glm::vec2 m_cPosition;
		glm::mat4 m_cCameraMatrix;

		int m_iScreenWidth;
		int m_iScreenHeight;

		glm::mat4 m_cOrthoMatrix;
	};
}

