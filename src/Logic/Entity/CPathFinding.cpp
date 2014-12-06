#include "CPathFinding.h"
#include "../Map/MapData.h"
#include <algorithm>

PathFinding::CPathFinding::CPathFinding(const _mapTileAll *map) : m_pInGameMap(map)
{
	Clear();
}

void	PathFinding::CPathFinding::Clear()
{
	for (int i = 0; i < MAP_HEIGHT_BIG / 32; ++i)
	{
		for (int j = 0; j < MAP_WIDTH_BIG / 32; ++j)
		{
			m_map[i][j].i = i, m_map[i][j].j = j;
			m_map[i][j].g = 10;
			m_map[i][j].h = 0;
			m_map[i][j].f = 0;
			m_map[i][j].parent = 0;
			m_map[i][j].type = NodeType::map;
		}
	}

	m_closedListIndex = 0;
	m_openListIndex = 0;
}

void	PathFinding::CPathFinding::Clear(int startI, int startJ, int searchWidth, int searchHeight)
{
	int	i = startI - searchHeight, j = startJ - searchWidth;

	if (i < 0)
		i = 0;

	if (j < 0)
		j = 0;

	int	finishI = startI + searchHeight, finishJ = startJ + searchWidth;

	if (finishI >= m_mapHeight)
		finishI = m_mapHeight - 1;

	if (finishJ >= m_mapWidth)
		finishJ = m_mapWidth - 1;

	int k = j;

	for (; i <= finishI; ++i)
	{
		for (; j <= finishJ; ++j)
		{
			m_map[i][j].i = i, m_map[i][j].j = j;
			m_map[i][j].g = 10;
			m_map[i][j].h = 0;
			m_map[i][j].f = 0;
			m_map[i][j].parent = 0;
			m_map[i][j].type = NodeType::map;
		}

		j = k;
	}

	m_closedListIndex = 0;
	m_openListIndex = 0;
}

bool	PathFinding::CPathFinding::FindPath(int startI, int startJ, int searchWidth, int searchHeight, int goalI, int goalJ, PathFinding::AlgType type)
{
	if (type == AlgType::AStar)
		return	RunAStar(startI, startJ, searchWidth, searchHeight, goalI, goalJ);
	else if (type == AlgType::Dijkstra)
		return	RunDijkstra(startI, startJ, searchWidth, searchHeight, goalI, goalJ);

	return	false;
}

bool	PathFinding::CPathFinding::RunAStar(int startI, int startJ, int searchWidth, int searchHeight, int goalI, int goalJ)
{
	if (!UpdateMapWidthHeight())
		return	false;

	int	searchI{ startI }, searchJ{ startJ };

	m_map[searchI][searchJ].type = NodeType::open;

	m_openList[m_openListIndex] = m_map[searchI][searchJ];

	m_openListIndex++;

	bool	pathFound{ false };

	int		openListStart{ 0 };

	while (!pathFound)
	{
		int	lowestCost{ 999999 };
		int	lowestCostNodeIndex = -1;

		for (int i = openListStart; i < m_openListIndex; ++i)
		{
			if (m_openList[i].f < lowestCost && m_openList[i].type == NodeType::open)
			{
				lowestCost = m_openList[i].f;
				lowestCostNodeIndex = i;
			}
		}

		if (lowestCostNodeIndex == -1)
		{
			return	false;
		}

		m_openList[lowestCostNodeIndex].type = NodeType::closed;
		m_map[m_openList[lowestCostNodeIndex].i][m_openList[lowestCostNodeIndex].j].type = NodeType::closed;

		for (int i = openListStart; i < m_openListIndex; ++i)
		{
			if (m_openList[i].type != NodeType::open)
				openListStart++;
			else
				break;
		}

		m_closedList[m_closedListIndex] = m_openList[lowestCostNodeIndex];

		searchI = m_closedList[m_closedListIndex].i;
		searchJ = m_closedList[m_closedListIndex].j;

		if (searchI == goalI && searchJ == goalJ)
			pathFound = true;
		else
		{
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					int posI{ i + searchI }, posJ{ j + searchJ };

					if (posI >= 0 && posJ >= 0 && posI >= startI - searchHeight && posI < m_mapHeight && posI <= startI + searchHeight && posJ >= startJ - searchWidth && posJ < m_mapWidth && posJ <= startJ + searchWidth && m_map[posI][posJ].type != NodeType::wall && IsValidDiagonalMove(i, j, searchJ, searchI))
					{
						if (m_map[posI][posJ].type == NodeType::map)
						{
							m_map[posI][posJ].type = NodeType::open;

							m_openList[m_openListIndex] = m_map[posI][posJ];

							m_openListIndexMap[posI][posJ] = m_openListIndex;

							if ((i == -1 && j == -1) || (i == -1 && j == 1) || (i == 1 && j == -1) || (i == 1 && j == 1))
							{
								m_openList[m_openListIndex].g = 14;
							}
							else
								m_openList[m_openListIndex].g = 10;

							m_openList[m_openListIndex].g += m_closedList[m_closedListIndex].g; // add the cost of the parent
							m_openList[m_openListIndex].parent = m_closedListIndex;
							m_openList[m_openListIndex].h = CalcHEuclidean(posI, posJ, goalI, goalJ);
							m_openList[m_openListIndex].f = m_openList[m_openListIndex].g + m_openList[m_openListIndex].h;

							m_openListIndex++;
						}
						else if (m_map[posI][posJ].type == NodeType::open) // check if the current node (last node added on the closed list) is a batter parent
						{
							int index{ m_openListIndexMap[posI][posJ] };

							int oldGCost = m_openList[index].g - m_closedList[m_openList[index].parent].g;

							if (m_closedList[m_closedListIndex].g + oldGCost < m_openList[index].g)
							{
								m_openList[index].g -= m_closedList[m_openList[index].parent].g;
								m_openList[index].g += m_closedList[m_closedListIndex].g;

								m_openList[index].f = m_openList[index].g + m_openList[index].h;
								m_openList[index].parent = m_closedListIndex;
							}
						}
					}
				}
			}
		}

		m_closedListIndex++;
	}

	bool	pathExtracted{ false };

	int		pathIndex{ m_closedListIndex - 1 };

	while (!pathExtracted)
	{
		m_path.emplace_back(std::pair < int, int > {m_closedList[pathIndex].i, m_closedList[pathIndex].j});

		pathIndex = m_closedList[pathIndex].parent;

		if (m_closedList[pathIndex].i == startI && m_closedList[pathIndex].j == startJ)
		{
			pathExtracted = true;
		}
	}

	return	true;
}

bool	PathFinding::CPathFinding::RunDijkstra(int startI, int startJ, int searchWidth, int searchHeight, int goalI, int goalJ)
{
	if (!UpdateMapWidthHeight())
		return	false;

	int	searchI{ startI }, searchJ{ startJ };

	m_map[searchI][searchJ].type = NodeType::open;

	m_openList[m_openListIndex] = m_map[searchI][searchJ];

	m_openListIndex++;

	bool	pathFound{ false };

	int		openListStart{ 0 };

	while (!pathFound)
	{
		int	lowestCost{ 999999 };
		int	lowestCostNodeIndex = -1;

		for (int i = openListStart; i < m_openListIndex; ++i)
		{
			if (m_openList[i].g < lowestCost && m_openList[i].type == NodeType::open)
			{
				lowestCost = m_openList[i].g;
				lowestCostNodeIndex = i;
			}
		}

		if (lowestCostNodeIndex == -1)
		{
			return	false;
		}

		m_openList[lowestCostNodeIndex].type = NodeType::closed;
		m_map[m_openList[lowestCostNodeIndex].i][m_openList[lowestCostNodeIndex].j].type = NodeType::closed;

		for (int i = openListStart; i < m_openListIndex; ++i)
		{
			if (m_openList[i].type != NodeType::open && m_openList[i].type != NodeType::map)
				openListStart++;
			else
				break;
		}

		m_closedList[m_closedListIndex] = m_openList[lowestCostNodeIndex];

		searchI = m_closedList[m_closedListIndex].i;
		searchJ = m_closedList[m_closedListIndex].j;

		if (searchI == goalI && searchJ == goalJ)
			pathFound = true;
		else
		{
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					int posI{ i + searchI }, posJ{ j + searchJ };

					if (posI >= 0 && posJ >= 0 && posI >= startI - searchHeight && posI < m_mapHeight && posI <= startI + searchHeight && posJ >= startJ - searchWidth && posJ < m_mapWidth && posJ <= startJ + searchWidth && IsValidDiagonalMove(i, j, searchJ, searchI))
					{
						if (m_map[posI][posJ].type == NodeType::map)
						{
							m_map[posI][posJ].type = NodeType::open;

							m_openList[m_openListIndex] = m_map[posI][posJ];

							if ((i == -1 && j == -1) || (i == -1 && j == 1) || (i == 1 && j == -1) || (i == 1 && j == 1))
							{
								m_openList[m_openListIndex].g = 14;
							}
							else
								m_openList[m_openListIndex].g = 10;

							m_openList[m_openListIndex].g += m_closedList[m_closedListIndex].g; // add the cost of the parent
							m_openList[m_openListIndex].parent = m_closedListIndex;

							m_openListIndex++;
						}
					}
				}
			}
		}

		m_closedListIndex++;
	}

	bool	pathExtracted{ false };

	int		pathIndex{ m_closedListIndex - 1 };

	while (!pathExtracted)
	{
		m_path.emplace_back(std::pair < int, int > {m_closedList[pathIndex].i * 32, m_closedList[pathIndex].j * 32});

		pathIndex = m_closedList[pathIndex].parent;

		if (m_closedList[pathIndex].i == startI && m_closedList[pathIndex].j == startJ)
		{
//			m_path.emplace_back(std::pair < int, int > {m_closedList[pathIndex].i, m_closedList[pathIndex].j});
			pathExtracted = true;
		}
	}

	return	true;
}

void	PathFinding::CPathFinding::GetPath(std::vector<std::pair<int, int>> &path)
{
	path = std::move(m_path);
}

int	PathFinding::CPathFinding::CalcH(int i, int j, int goalI, int goalJ)
{
	return	(abs(i - goalI) + abs(j - goalJ)) * 10;
}

int		PathFinding::CPathFinding::CalcHEuclidean(int i, int j, int goalI, int goalJ)
{
	int x = abs(i - goalI), y = abs(j - goalJ);

	return	static_cast<int>(10 * sqrt(x * x + y * y));
}

int		PathFinding::CPathFinding::CalcHDiagonal(int i, int j, int goalI, int goalJ)
{
	int x = abs(i - goalI), y = abs(j - goalJ);

	return	10 * (x + y) + (14 - 2 * 10) * std::min(x, y);
}

bool	PathFinding::CPathFinding::IsValidDiagonalMove(int i, int j, int posX, int posY)
{
	if (i == -1 && j == -1) // top left movement (diagonal movement)...
	{
		// check if the node above or the node to the left are currently traversable
		if (m_map[posY - 1][posX].type == PathFinding::NodeType::wall || m_map[posY][posX - 1].type == PathFinding::NodeType::wall)
			return	false;
	}
	else if (i == -1 && j == 1) // top right movement (diagonal movement)
	{
		// check if the node above or to the right are currently traversable
		if (m_map[posY - 1][posX].type == PathFinding::NodeType::wall || m_map[posY][posX + 1].type == PathFinding::NodeType::wall)
			return	false;
	}
	else if (i == 1 && j == -1) // down left movement (diagonal movement)
	{
		// check if the node down and the node to the left are currently traversable
		if (m_map[posY][posX - 1].type == PathFinding::NodeType::wall || m_map[posY + 1][posX].type == PathFinding::NodeType::wall)
			return	false;
	}
	else if (i == 1 && j == 1) // down right movement (diagonal movement)
	{
		// check if the node down and the node to the right are currently traversable
		if (m_map[posY + 1][posX].type == PathFinding::NodeType::wall || m_map[posY][posX + 1].type == PathFinding::NodeType::wall)
			return	false;
	}

	return	true;
}

bool	PathFinding::CPathFinding::UpdateMapWidthHeight()
{
	MapType	type{ *m_pInGameMap->currentMap };

	if (type == MapType::small_)
	{
		m_mapWidth = MAP_WIDTH_SMALL / 32;
		m_mapHeight = MAP_HEIGHT_SMALL / 32;
	}
	else if (type == MapType::medium)
	{
		m_mapWidth = MAP_WIDTH_MEDIUM / 32;
		m_mapHeight = MAP_HEIGHT_MEDIUM / 32;
	}
	else if (type == MapType::big)
	{
		m_mapWidth = MAP_WIDTH_BIG / 32;
		m_mapHeight = MAP_HEIGHT_BIG / 32;
	}
	else
		return	false;

	return	true;
}
