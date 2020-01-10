
#include "GamePlayScreen.h"

#include <SnakEngine\IGameWorld.h>

#include <SDL\SDL.h>

CGamePlayScreen::CGamePlayScreen()
{}

CGamePlayScreen::~CGamePlayScreen()
{}

int CGamePlayScreen::GetNextScreenIndex() const
{
	return -1;
}

int CGamePlayScreen::GetPreviousScreenIndex() const
{
	return -1;
}

void CGamePlayScreen::CGamePlayScreen::Create()
{}

void CGamePlayScreen::CGamePlayScreen::Destroy()
{}

void CGamePlayScreen::OnStart()
{}

void CGamePlayScreen::OnExit()
{}

void CGamePlayScreen::Update()
{
	UpdateInput();
}

void CGamePlayScreen::Draw()
{}

void CGamePlayScreen::UpdateInput()
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt))
	{
		m_pGame->OnSDLEvent(evt);
	}
}