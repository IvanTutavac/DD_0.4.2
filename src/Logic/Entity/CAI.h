#pragma once

#include "../../Data/Const.h"

#include "AIData.h"

namespace PathFinding
{
	class CPathFinding;
}

struct _mapTileAll;
struct _entityWrapper;

struct	_attPos
{
	int	i, j;
	int	id; // -1 -> position is free for the taking
	bool blocked; // if true the position is not traversable
};

class CAI
{
private:

	PathFinding::CPathFinding	*m_pPathFinding;

	const _mapTileAll	*m_pMap;

	std::vector<_aiData>	m_aiData;
	_entityWrapper	*m_pEnemy; // just a pointer

	std::vector<std::pair<int, int>>	m_playerMapPos;

	_attPos	m_playerMeleeAttPosLeft;
	_attPos	m_playerMeleeAttPosRight;
	_attPos	m_playerMeleeAttPosUp;
	_attPos	m_playerMeleeAttPosDown;

	bool	m_floorPassable[IMG_FLOOR_FINISH + 1];

	// updates m_playerMapPos with tiles over which player is standing
	void	CalcPlayerMapPos(int playerX, int playerY);
	void	CalcMeleeAttPosOnPlayer(int playerX, int playerY);
	void	CalcMeleeAttPosOnPlayer(int i, int j, _attPos &pos);

	bool	GetFreeMeleeAttPosOnPlayer(int entityIndex, int &i, int &j);
	bool	GetFreeMeleeAttPosOnPlayer(int entityID, int &i, int &j, _attPos &pos);
	bool	AlreadyHasMeleeAttPosOnPlayer(int entityID, int &i, int &j, _attPos &pos);

	void	UpdateMeleeAttEndPath(int enemyIndex, std::pair<int, int> &pos, int playerX, int playerY);

	void	ClearAttPos(int entityIndex);

public:
	
	CAI() = delete;
	CAI(const _mapTileAll *map, _entityWrapper *enemy, bool floorPassable[]);
	~CAI();

	bool	Init();
	void	Clean();

	bool	DeleteEnemy(int index);
	void	AddEnemy();

	bool	UpdateAI(int playerX, int playerY);

	std::vector<_aiData>&	GetAIData();
};
