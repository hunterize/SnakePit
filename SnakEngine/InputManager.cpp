#include "InputManager.h"

namespace SnakEngine
{
	InputManager::InputManager() : m_mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::PressKey(unsigned int keyID)
	{
		m_keyMap[keyID] = true;
	}

	void InputManager::ReleaseKey(unsigned int keyID)
	{
		m_keyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		return CheckKeyState(keyID, m_keyMap);
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		return CheckKeyState(keyID, m_keyMap) && !CheckKeyState(keyID, m_previousKeyMap);
	}

	//get previous key status
	void InputManager::Update()
	{
		for(auto& it : m_keyMap)
		//for each (auto it in m_keyMap)
		{
			m_previousKeyMap[it.first] = it.second;
		}
	}

	void InputManager::SetMouseCoords(float x, float y)
	{
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	glm::vec2 InputManager::GetMouseCoords()
	{
		return m_mouseCoords;
	}

	bool InputManager::CheckKeyState(unsigned int keyID, std::unordered_map<unsigned int, bool>& map)
	{
		auto it = map.find(keyID);
		if (it != map.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}
}