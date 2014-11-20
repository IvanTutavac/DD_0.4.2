#include "CMapInGame.h"

CMapInGame::CMapInGame()
{
	m_mapState = MapInGameState::nothing;
}

CMapInGame::~CMapInGame()
{

}

bool	CMapInGame::Init()
{
	CMap::Init();

	return	true;
}

void	CMapInGame::Clean()
{
	CMap::Clean();
}

bool	CMapInGame::IsMapShown()
{
	if (m_mapState == MapInGameState::showMap)
		return	true;
	return	false;
}

void	CMapInGame::SetMapShown()
{
	m_mapState = MapInGameState::showMap;
}
