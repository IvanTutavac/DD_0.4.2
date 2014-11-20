#include "CState.h"
#include "StateData.h"

CState::CState()
{
	m_gameState = GameState::nothing;
}

CState::~CState()
{

}

bool	CState::Init()
{
	return	true;
}

void	CState::Clean()
{

}

bool	CState::IsGameStateInGame() const
{
	if (m_gameState == GameState::inGame)
		return	true;
	return	false;
}

void	CState::SetGameStateInGame()
{
	m_gameState = GameState::inGame;
}

bool	CState::IsGameStateMainMenu() const
{
	if (m_gameState == GameState::mainMenu)
		return	true;
	return	false;
}

void	CState::SetGameStateMainMenu()
{
	m_gameState = GameState::mainMenu;
}

bool	CState::IsGameStateMainOptions() const
{
	if (m_gameState == GameState::mainOptions)
		return	true;
	return	false;
}

void	CState::SetGameStateMainOptions()
{
	m_gameState = GameState::mainOptions;
}

bool	CState::IsGameStateMapEditor() const
{
	if (m_gameState == GameState::mapEditor)
		return	true;
	return	false;
}

void	CState::SetGameStateMapEditor()
{
	m_gameState = GameState::mapEditor;
}
