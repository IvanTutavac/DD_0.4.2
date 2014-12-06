#pragma once

#include <vector>
#include "../../Data/Const.h"

struct _mapTileAll;

namespace PathFinding
{
	enum class NodeType
	{
		map, open, closed, wall
	};

	enum class AlgType
	{
		Dijkstra, AStar
	};

	struct _node
	{
		int	i, j;
		int	f, g, h;
		int	parent;
		NodeType	type;
	};

	class CPathFinding
	{
	private:

		_node	m_map[MAP_HEIGHT_BIG/32][MAP_WIDTH_BIG/32];
		_node	m_openList[MAP_HEIGHT_BIG/32 * MAP_WIDTH_BIG/32];
		_node	m_closedList[MAP_HEIGHT_BIG/32 * MAP_WIDTH_BIG/32];

		std::vector<std::pair<int, int>>	m_path;

		int		m_openListIndex;
		int		m_closedListIndex;

		/* this 2D array contains indexes to m_openList, if a node is added to the open list, it's position in the open list will be added here
		no need to clear because for an element to be accessed, it must first be added (used for A*)*/
		int		m_openListIndexMap[MAP_HEIGHT_BIG/32][MAP_WIDTH_BIG/32];

		const _mapTileAll	*m_pInGameMap;

		int		m_mapWidth;
		int		m_mapHeight;

	private:

		bool	UpdateMapWidthHeight();

		int		CalcH(int i, int j, int goalI, int goalJ);
		int		CalcHEuclidean(int i, int j, int goalI, int goalJ);
		int		CalcHDiagonal(int i, int j, int goalI, int goalJ);

		bool	IsValidDiagonalMove(int i, int j, int posX, int posY);

		bool	RunAStar(int startI, int startJ, int searchWidth, int searchHeight, int goalI, int goalJ);
		bool	RunDijkstra(int startI, int startJ, int searchWidth, int searchHeight, int goalI, int goalJ);

	public:

		CPathFinding() = delete;
		CPathFinding(const _mapTileAll *map);

		// resets the whole map and internal data
		void	Clear();

		bool	FindPath(int startI, int startJ, int searchWidth, int searchHeight, int goalI, int goalJ, AlgType type);
		
		// After FindPath has been called once, for each new call of FindPath, one must call Clear(int,int,int,int) first
		// it will reset the map part which has been used for pathfinding and internal data
		void	Clear(int startI, int startJ, int searchWidth, int searchHeight);

		// valid once per FindPath()
		void	GetPath(std::vector<std::pair<int, int>> &path);
	};
}

