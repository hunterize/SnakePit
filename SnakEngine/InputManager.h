#pragma once

#include <unordered_map>
#include <glm\glm.hpp>

namespace SnakEngine
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void Update();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		void SetMouseCoords(float x, float y);

		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 GetMouseCoords();

	private:
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_previousKeyMap;
		glm::vec2 m_mouseCoords;
		bool CheckKeyState(unsigned int keyID, std::unordered_map<unsigned int, bool>& map);

	};
}

