#pragma once

#include "CMap.h"

enum class MapInGameState
{
	nothing,
	showMap
};

class CMapInGame : public CMap
{
	MapInGameState	m_mapState;

public:

	CMapInGame();
	~CMapInGame();

	bool	Init();
	void	Clean();

	bool	IsMapShown();
	void	SetMapShown();
};
