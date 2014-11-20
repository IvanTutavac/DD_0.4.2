#pragma once

#include "CMap.h"
#include <map>

// addig new states -> update IsInImageWindow() GetCurrentImgMap(), init needs to be updated with a new call to InitMapTiles with the new image type,
// GetCurrentMapWidth and Height, SetHUDTile 

enum class MapEditorState
{
	nothing,
	showMap, 
	showTiles,
	showMonsters
};

class CMapEditor : public CMap
{
private:

	// for each new _map
	 
	// map that contains every tile img name
	_mapEditorImg	m_mapTiles;
	_mapEditorImg	m_mapMonsters;

	int				m_selectedTile; // mapEditor uses this variable as an imgIndex of the tile that will be inserted
	MapEditorState	m_selectTileType; // used with m_selectedTile to know which tile type it is
	std::map<MapEditorState, int>	m_HUDTile; // on a tile selection window, used for drawing the tile of this index and type

	MapEditorState	m_state;

	void	InitMapTiles(_mapEditorImg &map,const int lastImgN);

public:

	CMapEditor();
	~CMapEditor();

	bool	Init();
	void	Clean();

public:

	bool	IsInMap();
	void	SetInMap();

	bool	IsInAllTiles();
	void	SetInAllTiles();

	// it will be read from mapTile
	void	SetHUDTile(int mouseX,int mouseY);
	// default value is 0, depending on which image selection window is active, its hud tile will be returned
	int		GetHUDTile();

	// the current map will be updated with the selectedTile
	void	InsertTileToMap(int mouseX, int mouseY);
	void	InsertTileToList(int mouseX, int mouseY);
	void	RemoveTileFromList(int mouseX, int mouseY);

	// takes value from m_HUDTile
	void	SetSelectedTile();
	int		GetSelectedTile();

	bool	IsInAllMonsters();
	void	SetInAllMonsters();

	// returns true if the current window is a image selection one, otherwise returns false
	bool	IsInImageWindow();
	bool	IsSelectedImageFloor();

	int		GetCurrentMapWidth() const override;
	int		GetCurrentMapHeight() const override;

	_mapEditorImg*	GetMapTiles();
	_mapEditorImg*	GetMapMonsters();

	// returns the current image selection window map
	// returns nullptr on error and writes to log
	_mapEditorImg*	GetCurrentImgMap();

	// returns the TYPE_IMG of the current window which is used by the render to know which images to render on a image selection window
	// returns -1 on error and writes to log
	int				GetCurrentImgType();
};
