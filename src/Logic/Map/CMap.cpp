#include "CMap.h"
#include "../../Data/Globals.h"
#include "../../Tools/Log.h"
#include "../../Tools/StringWrapper.h"
#include <fstream>

CMap::CMap()
{

}

CMap::~CMap()
{

}

bool	CMap::Init()
{
	m_mapFileName.clear();
	m_mapFileSize.clear();

	m_selectedMap = MapType::nothing;

	InitCamera();

	EmptyAllMaps();

	m_currentMap.small_ = &m_mapSmall;
	m_currentMap.medium = &m_mapMedium;
	m_currentMap.big = &m_mapBig;

	return	true;
}

void	CMap::Clean()
{
	
}

void	CMap::EmptyAllMaps()
{
	EmptySmallMap(MapEmpty::everything);
	EmptyMediumMap(MapEmpty::everything);
	EmptyBigMap(MapEmpty::everything);
	EmptyOverworld();
}

void	CMap::EmptySmallMap(const MapEmpty &e)
{
	for (int i = 0; i < MAP_HEIGHT_SMALL / 32; i++)
	{
		for (int j = 0; j < MAP_WIDTH_SMALL / 32; j++)
		{
			if (e == MapEmpty::everything)
			{
				m_mapSmall.map[i][j].imgIndex = 0;
				m_mapSmall.map[i][j].entityIndex = -1;
			}
			else if (e == MapEmpty::entity)
			{
				m_mapSmall.map[i][j].entityIndex = -1;
			}
			else if (e == MapEmpty::img)
			{
				m_mapSmall.map[i][j].imgIndex = 0;
			}
		}
	}
}

void	CMap::EmptyMediumMap(const MapEmpty &e)
{
	for (int i = 0; i < MAP_HEIGHT_MEDIUM / 32; i++)
	{
		for (int j = 0; j < MAP_WIDTH_MEDIUM / 32; j++)
		{
			if (e == MapEmpty::everything)
			{
				m_mapMedium.map[i][j].imgIndex = 0;
				m_mapMedium.map[i][j].entityIndex = -1;
			}
			else if (e == MapEmpty::entity)
			{
				m_mapMedium.map[i][j].entityIndex = -1;
			}
			else if (e == MapEmpty::img)
			{
				m_mapMedium.map[i][j].imgIndex = 0;
			}
		}
	}
}

void	CMap::EmptyBigMap(const MapEmpty &e)
{
	for (int i = 0; i < MAP_HEIGHT_BIG / 32; i++)
	{
		for (int j = 0; j < MAP_WIDTH_BIG / 32; j++)
		{
			if (e == MapEmpty::everything)
			{
				m_mapBig.map[i][j].imgIndex = 0;
				m_mapBig.map[i][j].entityIndex = -1;
			}
			else if (e == MapEmpty::entity)
			{
				m_mapBig.map[i][j].entityIndex = -1;
			}
			else if (e == MapEmpty::img)
			{
				m_mapBig.map[i][j].imgIndex = 0;
			}
		}
	}
}

void	CMap::EmptyOverworld()
{

}

void	CMap::InitCamera()
{
	m_cameraX = (float)g_windowWidth / 2;
	m_cameraY = (float)g_windowHeight / 2;
}

float	CMap::GetCameraX() const
{
	return	m_cameraX;
}

void	CMap::SetCameraX(float cameraX)
{
	m_cameraX = cameraX;
}

float	CMap::GetCameraY() const
{
	return	m_cameraY;
}

void	CMap::SetCameraY(float cameraY)
{
	m_cameraY = cameraY;
}

float	CMap::GetPlayerX() const
{
	return	m_playerPos.x;
}

void	CMap::SetPlayerX(float value) 
{
	m_playerPos.x = value;
}

float	CMap::GetPlayerY() const
{
	return	m_playerPos.y;
}

void	CMap::SetPlayerY(float value)
{
	m_playerPos.y = value;
}

_mapPos&	CMap::GetPlayerMapPos()
{
	return	m_playerPos;
}

bool	CMap::IsMapSmall()
{
	if (m_selectedMap == MapType::small_)
		return	true;
	return false;
}

void	CMap::SetMapSmall()
{
	m_selectedMap = MapType::small_;
}

bool	CMap::IsMapMedium()
{
	if (m_selectedMap == MapType::medium)
		return	true;
	return	false;
}

void	CMap::SetMapMedium()
{
	m_selectedMap = MapType::medium;
}

bool	CMap::IsMapBig()
{
	if (m_selectedMap == MapType::big)
		return	true;
	return	false;
}

void	CMap::SetMapBig()
{
	m_selectedMap = MapType::big;
}

int		CMap::GetCurrentMapHeight() const
{
	if (m_selectedMap == MapType::medium)
	{
		return	MAP_HEIGHT_MEDIUM;
	}
	else if (m_selectedMap == MapType::small_)
	{
		return	MAP_HEIGHT_SMALL;
	}
	else if (m_selectedMap == MapType::big)
	{
		return	MAP_HEIGHT_BIG;
	}

	return	0;
}

int		CMap::GetCurrentMapWidth() const
{
	if (m_selectedMap == MapType::medium)
	{
		return	MAP_WIDTH_MEDIUM;
	}
	else if (m_selectedMap == MapType::small_)
	{
		return	MAP_WIDTH_SMALL;
	}
	else if (m_selectedMap == MapType::big)
	{
		return	MAP_WIDTH_BIG;
	}

	return	0;
}

const _mapTileAll*	CMap::GetCurrentMap() 
{
	if (m_selectedMap == MapType::big)
	{
		m_currentMap.currentMap = MapType::big;
	}
	else if (m_selectedMap == MapType::medium)
	{
		m_currentMap.currentMap = MapType::medium;
	}
	else if (m_selectedMap == MapType::small_)
	{
		m_currentMap.currentMap = MapType::small_;
	}

	return	&m_currentMap;
}

_mapTileSmall* CMap::GetMapSmall()
{
	return	&m_mapSmall;
}

_mapTileMedium* CMap::GetMapMedium()
{
	return	&m_mapMedium;
}

_mapTileBig* CMap::GetMapBig()
{
	return	&m_mapBig;
}

int		CMap::SaveSelectedMap()
{
	if (m_mapFileName.empty() || m_mapFileSize.empty())
	{
		return	0;
	}

	std::fstream	dat;

	dat.open(m_mapFileName, std::ios::out | std::ios::binary);

	char fileSize[m_fileSizeLengthTmp];

	DD_Strcpy(fileSize, sizeof(fileSize), m_mapFileSize.c_str());

	dat.write((char*)&fileSize, sizeof(fileSize));

	if (IsMapSmall())
	{
		dat.write((char*)&m_mapSmall.map, sizeof(m_mapSmall.map));
	}
	else if (IsMapMedium())
	{
		dat.write((char*)&m_mapMedium.map, sizeof(m_mapMedium.map));
	}
	else if (IsMapBig())
	{
		dat.write((char*)&m_mapBig.map, sizeof(m_mapBig.map));
	}
	else
	{
		Log("CMap::SaveSelectedMap failed ... No map size set !?");
		return	-1;
	}

	size_t enemyNum = m_enemies.size();

	dat.write((char*)&enemyNum, sizeof(enemyNum));

	for (const auto& enemy : m_enemies)
	{
		dat.write((char*)&enemy, sizeof(enemy));
	}

	dat.clear();
	dat.close();

	return	1;
}

bool	CMap::LoadMap(const std::string &fileName_)
{
	std::fstream	dat;

	std::string		fullFileName = "data/maps/" + fileName_;

	m_mapFileName = fullFileName;

	dat.open(fullFileName, std::ios::in | std::ios::binary);

	if (!dat.is_open())
	{
		dat.clear();
		dat.close();
		Log("CMap::LoadMap() failed");
		return	false;
	}

	char fileSize[m_fileSizeLengthTmp];

	dat.read((char*)&fileSize, sizeof(fileSize));

	m_mapFileSize = fileSize;

	if (!CreateNewMap())
	{
		dat.clear();
		dat.close();
		Log("CMap::LoadMap() failed");
		return	false;
	}

	// map load

	if (IsMapSmall())
	{
		dat.read((char*)&m_mapSmall.map, sizeof(m_mapSmall.map));
	}
	else if (IsMapMedium())
	{
		dat.read((char*)&m_mapMedium.map, sizeof(m_mapMedium.map));
	}
	else if (IsMapBig())
	{
		dat.read((char*)&m_mapBig.map, sizeof(m_mapBig.map));
	}
	else
	{
		Log("CMap::LoadMap failed ... No map size set !?");
		return	false;
	}

	// enemy load

	size_t enemyNum{};

	dat.read((char*)&enemyNum, sizeof(enemyNum));

	m_enemies.reserve(enemyNum);

	_mapPos	enemy{};

	for (size_t i = 0; i < enemyNum; i++)
	{
		dat.read((char*)&enemy, sizeof(enemy));

		m_enemies.push_back(enemy);
	}

	dat.close();
	dat.clear();

	InitCamera();

	return		true;
}

void	CMap::SetMapFileName(const std::string &fileName)
{
	m_mapFileName = "data/maps/";
	m_mapFileName += fileName;
	m_mapFileName += ".DDmap";
}

void	CMap::SetMapFileSize(const std::string &fileSize)
{
	if (fileSize == "Small" || fileSize == "Medium" || fileSize == "Big")
	{
		m_mapFileSize = fileSize;
	}
	else
	{
		m_mapFileSize.clear();
	}
}

bool	CMap::CreateNewMap()
{
	m_enemies.clear();

	if (m_mapFileSize == "Small")
	{
		SetMapSmall();
		EmptySmallMap(MapEmpty::everything);
	}
	else if (m_mapFileSize == "Medium")
	{
		SetMapMedium();
		EmptyMediumMap(MapEmpty::everything);
	}
	else if (m_mapFileSize == "Big")
	{
		SetMapBig();
		EmptyBigMap(MapEmpty::everything);
	}
	else
	{
		return	false;
		Log("CMap::CreateNewMap failed, wrong fileSize");
	}

	return	true;
}

std::string	CMap::GetFileName()
{
	if (m_mapFileName.size() < 1)
		return "";

	std::string tmp = m_mapFileName;
	std::string str = "data/maps/";
	std::string fileName = tmp.replace(tmp.find(str), str.length(), "");

	return	fileName;
}

const std::vector<_mapPos>*	CMap::GetEnemyMapPos() const
{
	return	&m_enemies;
}

void	CMap::DeleteEnemyMapPos()
{
	m_enemies.clear();
}

void	CMap::UpdateXY(int &x, int &y)
{
	int k = (int)(y + m_cameraY - g_windowHeight / 2);
	int l = (int)(x + m_cameraX - g_windowWidth / 2);

	y = k;
	x = l;
}
