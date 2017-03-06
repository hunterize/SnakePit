#pragma once
#include <SnakEngine\IGameWorld.h>
#include "GamePlayScreen.h"

class CApp : public SnakEngine::IGameWorld
{
public:
	CApp();
	~CApp();

	virtual void OnInit() override;
	virtual void OnExit() override;
	virtual void AddScreens() override;

private:
	std::unique_ptr<CGamePlayScreen> m_pGamePlayScreen = nullptr;
};

