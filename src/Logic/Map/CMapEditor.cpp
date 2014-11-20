#include "CMapEditor.h"
#include "../Physics/CollisionFunctions.h"
#include "../../Data/Enums.h"
#include "../../Data/Globals.h"
#include "../../Data/Const.h"
#include "../../Tools/Log.h"

CMapEditor::CMapEditor()
{

}

CMapEditor::~CMapEditor()
{

}

bool	CMapEditor::Init()
{
	CMap::Init();

	m_state = MapEditorState::nothing;

	m_HUDTile[MapEditorState::showTiles] = 0;
	m_selectedTile = 0;
	m_selectTileType = MapEditorState::showTiles;

	InitMapTiles(m_mapTiles, IMG_FLOOR_FINISH);
	InitMapTiles(m_mapMonsters, IMG_MONSTER_FINISH);

	return	true;
}

void	CMapEditor::Clean()
{
	CMap::Clean();
}

void	CMapEditor::InitMapTiles(_mapEditorImg &map, const int lastImgN)
{
	int imgNumber = 0;

	for (int i = 0; i < MAP_HEIGHT_SMALL / 32; i++)
	{
		for (int j = 0; j < MAP_WIDTH_SMALL / 32; j++)
		{
			// check if we have imgs left to put in map
			if (imgNumber <= lastImgN)
			{
				map.map[i][j] = imgNumber;
				imgNumber++;
			}
			else // we've put all img files 
			{
				map.map[i][j] = 0; // black img 
			}
		}
	}
}

bool	CMapEditor::IsInMap()
{
	if (m_state == MapEditorState::showMap)
		return	true;
	return	false;
}

void	CMapEditor::SetInMap()
{
	m_state = MapEditorState::showMap;
}

bool	CMapEditor::IsInAllTiles()
{
	if (m_state == MapEditorState::showTiles)
		return	true;
	return	false;
}

void	CMapEditor::SetInAllTiles()
{
	m_state = MapEditorState::showTiles;
}

bool	CMapEditor::IsInAllMonsters()
{
	if (m_state == MapEditorState::showMonsters)
		return	true;
	return	false;
}

void	CMapEditor::SetInAllMonsters()
{
	m_state = MapEditorState::showMonsters;
}

bool	CMapEditor::IsInImageWindow()
{
	if (m_state == MapEditorState::showMonsters || m_state == MapEditorState::showTiles)
		return	true;
	return	false;
}

bool	CMapEditor::IsSelectedImageFloor()
{
	if (m_selectTileType == MapEditorState::showTiles)
	{
		return	true;
	}

	return	false;
}

_mapEditorImg*	CMapEditor::GetMapTiles()
{
	return	&m_mapTiles;
}

_mapEditorImg*	CMapEditor::GetCurrentImgMap()
{
	if (m_state == MapEditorState::showMonsters)
	{
		return	&m_mapMonsters;
	}
	else if (m_state == MapEditorState::showTiles)
	{
		return	&m_mapTiles;
	}

	Log("CMapEditor::GetCurrentImgMap() failed, wrong state: ", static_cast<int>(m_state));
	return	nullptr;
}

int				CMapEditor::GetCurrentImgType()
{
	if (m_state == MapEditorState::showMonsters)
	{
		return	TYPE_IMG_MON;
	}
	else if (m_state == MapEditorState::showTiles)
	{
		return	TYPE_IMG_FLOOR;
	}

	Log("CMapEditor::GetCurrentImgType() failed, wrong state: ", static_cast<int>(m_state));
	return	-1;
}

_mapEditorImg*	CMapEditor::GetMapMonsters()
{
	return	&m_mapMonsters;
}

int	CMapEditor::GetCurrentMapWidth() const
{
	if (m_state == MapEditorState::showMap)
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
	}
	else if (m_state == MapEditorState::showTiles)
	{
		return	MAP_WIDTH_SMALL;
	}
	else if (m_state == MapEditorState::showMonsters)
	{
		return	MAP_WIDTH_SMALL;
	}

	return	0;
}

int	CMapEditor::GetCurrentMapHeight() const
{
	if (m_state == MapEditorState::showMap)
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
	}
	else if (m_state == MapEditorState::showTiles)
	{
		return	MAP_HEIGHT_SMALL;
	}
	else if (m_state == MapEditorState::showMonsters)
	{
		return	MAP_HEIGHT_SMALL;
	}

	return	0;
}

void	CMapEditor::SetSelectedTile()
{
	if (m_state == MapEditorState::showMonsters)
	{
		m_selectedTile = m_HUDTile[MapEditorState::showMonsters];
		m_selectTileType = MapEditorState::showMonsters;
	}
	else if (m_state == MapEditorState::showTiles)
	{
		m_selectedTile = m_HUDTile[MapEditorState::showTiles];
		m_selectTileType = MapEditorState::showTiles;
	}
}

int		CMapEditor::GetSelectedTile()
{
	return	m_selectedTile;
}

void	CMapEditor::SetHUDTile(int mouseX, int mouseY)
{
	if (m_state == MapEditorState::showMonsters)
	{
		m_HUDTile[MapEditorState::showMonsters] = m_mapMonsters.map[mouseY / TILE_SIZE][mouseX / TILE_SIZE];
	}
	else if (m_state == MapEditorState::showTiles)
	{
		m_HUDTile[MapEditorState::showTiles] = m_mapTiles.map[mouseY / TILE_SIZE][mouseX / TILE_SIZE];
	}

}

int		CMapEditor::GetHUDTile()
{
	if (m_state == MapEditorState::showMonsters)
	{
		return	m_HUDTile[MapEditorState::showMonsters];
	}
	else if (m_state == MapEditorState::showTiles)
	{
		return	m_HUDTile[MapEditorState::showTiles];
	}

	return	0;
}

void	CMapEditor::InsertTileToMap(int mouseX, int mouseY)
{
	UpdateXY(mouseX, mouseY);

	if (IsMapSmall())
	{
		m_mapSmall.map[mouseY / TILE_SIZE][mouseX / TILE_SIZE].imgIndex = m_selectedTile;
	}
	else if (IsMapMedium())
	{
		m_mapMedium.map[mouseY / TILE_SIZE][mouseX / TILE_SIZE].imgIndex = m_selectedTile;
	}
	else if (IsMapBig())
	{
		m_mapBig.map[mouseY / TILE_SIZE][mouseX / TILE_SIZE].imgIndex = m_selectedTile;
	}
}

void	CMapEditor::InsertTileToList(int mouseX, int mouseY)
{
	UpdateXY(mouseX, mouseY);

	if (m_selectTileType == MapEditorState::showMonsters)
	{
		_mapPos	pos{};
		pos.index = m_selectedTile;
		pos.id = CGlobal::GetEntityID();
		pos.x = static_cast<float>(mouseX);
		pos.y = static_cast<float>(mouseY);

		m_enemies.push_back(pos);
	}
}

void	CMapEditor::RemoveTileFromList(int mouseX, int mouseY)
{
	UpdateXY(mouseX, mouseY);

	for (int i = m_enemies.size() - 1; i >= 0; i--)
	{
		if (DD::CheckMouseCollision(mouseX, mouseY, static_cast<int>(m_enemies[i].x), static_cast<int>(m_enemies[i].y), 32, 32))
		{
			m_enemies.erase(m_enemies.begin() + i);
			return;
		}
	}
}
