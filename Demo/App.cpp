#include "App.h"



CApp::CApp()
{
}


CApp::~CApp()
{
}


void CApp::OnInit()
{}
void CApp::OnExit()
{}
void CApp::AddScreens()
{
	m_pGamePlayScreen = std::make_unique<CGamePlayScreen>();

	m_pScreenList->AddScreen(m_pGamePlayScreen.get());
	m_pScreenList->SetScreen(m_pGamePlayScreen->GetScreenIndex());
}