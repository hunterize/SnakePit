#pragma once
#include "Human.h"
#include "Bullet.h"
#include "Gun.h"

#include <SnakEngine\InputManager.h>
#include <SnakEngine\Camera2D.h>

#include <SDL\SDL.h>

class Player : public Human
{
public:
	Player();
	virtual ~Player();

	void Init(float speed, glm::vec2 pos, SnakEngine::InputManager* inputManager, SnakEngine::Camera2D* camera, std::vector<Bullet>* bullets, float health);
	virtual void Update(const std::vector<std::string>& levelData, float elapseTime) override;

	void AddGun(Gun* gun);

protected:
	SnakEngine::InputManager* m_pInputManager;
	SnakEngine::Camera2D* m_pCamera;

	std::vector<Bullet>* m_pBullets;

	std::vector<Gun*> m_cGuns;
	int m_iCurrentGun;
};

