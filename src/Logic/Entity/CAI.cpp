#include "CAI.h"
#include "CPathFinding.h"
#include "EntityWrapper.h"
#include "../../Tools/debug.h"
#include "../../Tools/Log.h"

CAI::CAI(const _mapTileAll *map, _entityWrapper *enemy, bool floorPassable[]) : m_pMap(map), m_pEnemy(enemy)
{
	m_pPathFinding = nullptr;
	m_playerMeleeAttPosDown.id = -1;
	m_playerMeleeAttPosDown.blocked = false;
	m_playerMeleeAttPosLeft.id = -1;
	m_playerMeleeAttPosLeft.blocked = false;
	m_playerMeleeAttPosRight.id = -1;
	m_playerMeleeAttPosRight.blocked = false;
	m_playerMeleeAttPosUp.id = -1;
	m_playerMeleeAttPosUp.blocked = false;

	std::copy(floorPassable, floorPassable + IMG_FLOOR_FINISH + 1, m_floorPassable);
}

CAI::~CAI()
{

}

bool	CAI::Init()
{
	m_pPathFinding = DD_NEW PathFinding::CPathFinding{ m_pMap };

	return	true;
}

void	CAI::Clean()
{
	DD_DELETE(m_pPathFinding);
}

bool	CAI::DeleteEnemy(int index)
{
	if (index < 0 || index >= static_cast<int>(m_aiData.size()))
	{
		Log("CAI::DeleteEnemy(int) failed, wrong index: ", index);
		return	false;
	}

	m_aiData.erase(m_aiData.begin() + index);

	ClearAttPos(index);
	
	return	true;
}

void	CAI::AddEnemy()
{
	m_aiData.push_back(_aiData{});
}

static bool CheckDistance(int x1, int y1, int x2, int y2, int  distance)
{
	float x = static_cast<float>(abs(x2 - x1)), y = static_cast<float>(abs(y2 - y1));

	if (static_cast<int>(sqrtf(x*x + y*y)) < distance)
		return	true;

	return	false;
}

bool	CAI::UpdateAI(int playerX, int playerY)
{
	if (m_pEnemy->entity.empty())
		return	true;

	int entityIndex = 0, endI, endJ;

	if (CheckDistance(playerX, playerY, static_cast<int>(m_pEnemy->pos[entityIndex].x), static_cast<int>(m_pEnemy->pos[entityIndex].y), 45))
	{
		if (m_aiData[entityIndex].path.empty()) // entity not moving (meaning, movement disabled or goal reached) 
			m_entitiesReadyForAttack.emplace_back(entityIndex);
		
		return true;
	}

	if (m_aiData[entityIndex].playerX == playerX && m_aiData[entityIndex].playerY == playerY) // player has not moved, no need to find a new path
	{
		return	true;
	}

	CalcPlayerMapPos(playerX, playerY);

	CalcMeleeAttPosOnPlayer(playerX, playerY);

	if (m_aiData[entityIndex].movementDisabled)
	{
		m_aiData[entityIndex].path.clear();
		return	true;
	}

	if (GetFreeMeleeAttPosOnPlayer(entityIndex, endI, endJ))
	{
		int startI = static_cast<int>(m_pEnemy->pos[entityIndex].y / 32), startJ = static_cast<int>(m_pEnemy->pos[entityIndex].x / 32);
		int searchWidth{ 30 }, searchHeight{ 30 };

		m_pPathFinding->AddWalls(m_playerMapPos);
		
		if (m_pPathFinding->FindPath(startI, startJ,searchWidth, searchHeight, endI, endJ, PathFinding::AlgType::Dijkstra))
		{
			m_pPathFinding->GetPath(m_aiData[entityIndex].path);

			m_pEnemy->pos[entityIndex].endX = -1; // to force unit vector creation
			
			UpdateMeleeAttEndPath(entityIndex, m_aiData[entityIndex].path[0], playerX, playerY);

			m_aiData[entityIndex].playerX = playerX, m_aiData[entityIndex].playerY = playerY;
			m_aiData[entityIndex].state = AIEntityState::moving;
		}

		m_pPathFinding->RemoveWalls(m_playerMapPos);

		m_pPathFinding->Clear(startI, startJ, searchWidth, searchHeight);		
	}

	return	true;
}

void	CAI::CalcPlayerMapPos(int playerX, int playerY)
{
	m_playerMapPos.clear();

	m_playerMapPos.emplace_back(std::pair < int, int > {playerY / 32, playerX / 32});

	int	tX = (playerX / 32) * 32, tY = (playerY / 32) * 32;

	if (playerX > tX)
	{
		m_playerMapPos.emplace_back(std::pair < int, int > {playerY / 32, (playerX + 32) / 32});

		if (playerY > tY)
		{
			m_playerMapPos.emplace_back(std::pair < int, int > {(playerY + 32) / 32, playerX / 32});
			m_playerMapPos.emplace_back(std::pair < int, int > {(playerY + 32) / 32, (playerX + 32) / 32});
		}
	}
	else if (playerY > tY)
	{
		m_playerMapPos.emplace_back(std::pair < int, int > {(playerY + 32) / 32, playerX / 32});
	}
}

void	CAI::CalcMeleeAttPosOnPlayer(int playerX, int playerY)
{
	if (m_playerMapPos.size() == 1)
	{
		int i{ m_playerMapPos[0].first }, j{ m_playerMapPos[0].second };
		
		CalcMeleeAttPosOnPlayer(i - 1, j, m_playerMeleeAttPosUp);
		CalcMeleeAttPosOnPlayer(i + 1, j, m_playerMeleeAttPosDown);
		CalcMeleeAttPosOnPlayer(i, j - 1, m_playerMeleeAttPosLeft);
		CalcMeleeAttPosOnPlayer(i, j + 1, m_playerMeleeAttPosRight);
	}
	else if (m_playerMapPos.size() == 2)
	{
		if (m_playerMapPos[0].second == m_playerMapPos[1].second) // []
		{	
															 // [x] - player mostly in this node
			if (m_playerMapPos[0].first * 32 + 16 > playerY) // [ ]
			{
				int i{ m_playerMapPos[0].first }, j{ m_playerMapPos[0].second };

				CalcMeleeAttPosOnPlayer(i - 1, j, m_playerMeleeAttPosUp);
				CalcMeleeAttPosOnPlayer(i + 2, j, m_playerMeleeAttPosDown);
				CalcMeleeAttPosOnPlayer(i, j - 1, m_playerMeleeAttPosLeft);
				CalcMeleeAttPosOnPlayer(i, j + 1, m_playerMeleeAttPosRight);
			}	 // [ ]
			else // [x]
			{
				int i{ m_playerMapPos[1].first }, j{ m_playerMapPos[0].second };

				CalcMeleeAttPosOnPlayer(i - 2, j, m_playerMeleeAttPosUp);
				CalcMeleeAttPosOnPlayer(i + 1, j, m_playerMeleeAttPosDown);
				CalcMeleeAttPosOnPlayer(i, j - 1, m_playerMeleeAttPosLeft);
				CalcMeleeAttPosOnPlayer(i, j + 1, m_playerMeleeAttPosRight);
			}
		}
		else // [x][] or [][x]
		{
			if (m_playerMapPos[0].second * 32 + 16 > playerX) // [x][ ]
			{
				int i{ m_playerMapPos[0].first }, j{ m_playerMapPos[0].second };

				CalcMeleeAttPosOnPlayer(i - 1, j, m_playerMeleeAttPosUp);
				CalcMeleeAttPosOnPlayer(i + 1, j, m_playerMeleeAttPosDown);
				CalcMeleeAttPosOnPlayer(i, j - 1, m_playerMeleeAttPosLeft);
				CalcMeleeAttPosOnPlayer(i, j + 2, m_playerMeleeAttPosRight);
			}
			else // [ ][x]
			{
				int i{ m_playerMapPos[0].first }, j{ m_playerMapPos[1].second };

				CalcMeleeAttPosOnPlayer(i - 1, j, m_playerMeleeAttPosUp);
				CalcMeleeAttPosOnPlayer(i + 1, j, m_playerMeleeAttPosDown);
				CalcMeleeAttPosOnPlayer(i, j - 2, m_playerMeleeAttPosLeft);
				CalcMeleeAttPosOnPlayer(i, j + 2, m_playerMeleeAttPosRight);
			}
		}
	}
	else if (m_playerMapPos.size() == 4)
	{
		int i = static_cast<int>(round(playerY / 32)), j = static_cast<int>(round(playerX / 32));

		if (i == m_playerMapPos[0].first && j == m_playerMapPos[0].second) // upper left position
		{
			CalcMeleeAttPosOnPlayer(i - 1, j, m_playerMeleeAttPosUp);
			CalcMeleeAttPosOnPlayer(i + 2, j, m_playerMeleeAttPosDown);
			CalcMeleeAttPosOnPlayer(i, j - 1, m_playerMeleeAttPosLeft);
			CalcMeleeAttPosOnPlayer(i, j + 2, m_playerMeleeAttPosRight);
		}
		else if (i == m_playerMapPos[1].first && j == m_playerMapPos[1].second) // upper right position
		{
			CalcMeleeAttPosOnPlayer(i - 1, j, m_playerMeleeAttPosUp);
			CalcMeleeAttPosOnPlayer(i + 2, j, m_playerMeleeAttPosDown);
			CalcMeleeAttPosOnPlayer(i, j - 2, m_playerMeleeAttPosLeft);
			CalcMeleeAttPosOnPlayer(i, j + 1, m_playerMeleeAttPosRight);
		}
		else if (i == m_playerMapPos[2].first && j == m_playerMapPos[2].second) // down left position
		{
			CalcMeleeAttPosOnPlayer(i - 2, j, m_playerMeleeAttPosUp);
			CalcMeleeAttPosOnPlayer(i + 1, j, m_playerMeleeAttPosDown);
			CalcMeleeAttPosOnPlayer(i, j - 1, m_playerMeleeAttPosLeft);
			CalcMeleeAttPosOnPlayer(i, j + 2, m_playerMeleeAttPosRight);
		}
		else if (i == m_playerMapPos[3].first && j == m_playerMapPos[3].second) // down right position
		{
			CalcMeleeAttPosOnPlayer(i - 2, j, m_playerMeleeAttPosUp);
			CalcMeleeAttPosOnPlayer(i + 1, j, m_playerMeleeAttPosDown);
			CalcMeleeAttPosOnPlayer(i, j - 2, m_playerMeleeAttPosLeft);
			CalcMeleeAttPosOnPlayer(i, j + 1, m_playerMeleeAttPosRight);
		}
	}
}

void	CAI::CalcMeleeAttPosOnPlayer(int i, int j, _attPos &pos)
{
	if ((*m_pMap->currentMap == MapType::small_ && m_floorPassable[m_pMap->small_->map[i][j].imgIndex] && i >= 0 && j >= 0 && i < MAP_HEIGHT_SMALL / 32 && j < MAP_WIDTH_SMALL / 32)
		|| (*m_pMap->currentMap == MapType::medium && m_floorPassable[m_pMap->medium->map[i][j].imgIndex] && i >= 0 && j >= 0 && i < MAP_HEIGHT_MEDIUM / 32 && j < MAP_WIDTH_MEDIUM / 32)
		|| (*m_pMap->currentMap == MapType::big && m_floorPassable[m_pMap->big->map[i][j].imgIndex] && i >= 0 && j >= 0 && i < MAP_HEIGHT_BIG / 32 && j < MAP_WIDTH_BIG / 32))
	{
		pos.i = i;
		pos.j = j;

		if (pos.blocked) // position now traversable
		{
			pos.id = -1;
			pos.blocked = false;
		}
	}
	else
	{
		pos.id = -1;
		pos.blocked = true;
	}
}

bool	CAI::GetFreeMeleeAttPosOnPlayer(int entityIndex, int &i, int &j)
{
	int entityID  = m_pEnemy->entity[entityIndex].GetId();

	if (AlreadyHasMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosUp ))
		return	true;
	else if (AlreadyHasMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosDown))
		return	true;
	else if (AlreadyHasMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosRight))
		return	true;
	else if (AlreadyHasMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosLeft))
		return	true;
	else if (GetFreeMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosUp))
		return	true;
	else if (GetFreeMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosDown))
		return	true;
	else if (GetFreeMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosLeft))
		return	true;
	else if (GetFreeMeleeAttPosOnPlayer(entityID, i, j, m_playerMeleeAttPosRight))
		return	true;

	return	false;
}

bool	CAI::AlreadyHasMeleeAttPosOnPlayer(int entityID, int &i, int &j, _attPos &pos)
{
	if (pos.id == entityID)
	{
		i = pos.i, j = pos.j;
		
		return	true;
	}

	return	false;
}

bool	CAI::GetFreeMeleeAttPosOnPlayer(int entityID, int &i, int &j, _attPos &pos)
{
	if (!pos.blocked && pos.id == -1)
	{
		i = pos.i;
		j = pos.j;

		pos.id = entityID;

		return	true;
	}

	return	false;
}

void	CAI::UpdateMeleeAttEndPath(int enemyIndex, std::pair<int, int> &pos, int playerX, int playerY)
{
	int x = 32;

	int id = m_pEnemy->entity[enemyIndex].GetId();

	if (m_playerMeleeAttPosUp.id == id)
	{
		pos.first = playerY - x;
		pos.second = playerX;
	}
	else if (m_playerMeleeAttPosDown.id == id)
	{
		pos.first = playerY + x;
		pos.second = playerX;
	}
	else if (m_playerMeleeAttPosLeft.id == id)
	{
		pos.second = playerX - x;
		pos.first = playerY;
	}
	else if (m_playerMeleeAttPosRight.id == id)
	{
		pos.second = playerX + x;
		pos.first = playerY;
	}
}

void	CAI::ClearAttPos(int entityIndex)
{
	int id = m_pEnemy->entity[entityIndex].GetId();

	if (m_playerMeleeAttPosDown.id == id)
		m_playerMeleeAttPosDown.id = -1, m_playerMeleeAttPosDown.blocked = false;
	else if (m_playerMeleeAttPosLeft.id == id)
		m_playerMeleeAttPosLeft.id = -1, m_playerMeleeAttPosLeft.blocked = false;
	else if (m_playerMeleeAttPosRight.id == id)
		m_playerMeleeAttPosRight.id = -1, m_playerMeleeAttPosRight.blocked = false;
	else if (m_playerMeleeAttPosUp.id == id)
		m_playerMeleeAttPosUp.id = -1, m_playerMeleeAttPosUp.blocked = false;
}

std::vector<_aiData>&	CAI::GetAIData()
{
	return	m_aiData;
}

std::vector<int>&	CAI::GetEntitiesReadyForAttack()
{
	return	m_entitiesReadyForAttack;
}
