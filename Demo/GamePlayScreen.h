#pragma once

#include <SnakEngine\IGameScreen.h>

class CGamePlayScreen : public SnakEngine::IGameScreen
{
public:
	CGamePlayScreen();
	virtual ~CGamePlayScreen();

	virtual int GetNextScreenIndex() const override;
	virtual int GetPreviousScreenIndex() const override;

	virtual void Create() override;
	virtual void Destroy() override;

	virtual void OnStart() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Draw() override;

private:
	void UpdateInput();

};

