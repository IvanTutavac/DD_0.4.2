#pragma once

#include <vector>

#include "MapData.h"

// adding new sizes requires updating SetMapFileSize()

enum class MapType;
enum class MapEmpty;

class CMap
{
private:

	float			m_cameraX;
	float			m_cameraY;
	
	_mapPos			m_playerPos;

	std::string		m_mapFileName; // used for load/save
	std::string		m_mapFileSize; // used for load/save

	_mapTileAll		m_currentMap; // contains pointers to all maps and a variable for the current map size

protected:

	_mapTileSmall	m_mapSmall;
	_mapTileMedium	m_mapMedium;
	_mapTileBig		m_mapBig;

	static const	int		m_fileSizeLengthTmp = 20;

	MapType			m_selectedMap;

	std::vector<_mapPos>	m_enemies;

	// passed mouseX and mouseY will be updated with mapX and mapY
	void	UpdateXY(int &x, int &y);

public:

	CMap();
	virtual ~CMap();

	bool	Init();
	void	Clean();

	void	InitCamera();

	float	GetCameraX() const;
	void	SetCameraX(float cameraX);

	float	GetCameraY() const;
	void	SetCameraY(float cameraY);

	float	GetPlayerX() const;
	void	SetPlayerX(float value);

	float	GetPlayerY() const;
	void	SetPlayerY(float value);

	_mapPos&	GetPlayerMapPos();
	const _mapPos&	GetPlayerMapPosC() const;

	// returns 0 if something went wrong
	virtual int		GetCurrentMapWidth() const;
	// returns 0 if something went wrong
	virtual int		GetCurrentMapHeight() const;

	bool	IsMapSmall();
	bool	IsMapMedium();
	bool	IsMapBig();

	void	SetMapSmall();
	void	SetMapMedium();
	void	SetMapBig();

	_mapTileSmall*	GetMapSmall();
	_mapTileMedium* GetMapMedium();
	_mapTileBig*	GetMapBig();

	const _mapTileAll*	GetCurrentMap();

	void	EmptyAllMaps();
	void	EmptySmallMap(const MapEmpty &e);
	void	EmptyMediumMap(const MapEmpty &e);
	void	EmptyBigMap(const MapEmpty &e);
	void	EmptyOverworld(); // not yet implemented

	// uses internal map size and map name to save the name to data/maps/
	// returns -1 on error and writes to log
	// returns 0 if there was no file name or file size, save failed
	// returns 1 on success
	int		SaveSelectedMap();

	// Loads map, sets m_mapFileSize and m_mapFileName
	// calls InitCamera and sets the right state (call to CreateNewMap)
	// returns	false on error
	bool	LoadMap(const std::string  &fileName_);

	// Set the name of the map which will be used for saving the map, extension not needed, will be added
	void	SetMapFileName(const std::string &fileName);

	// Set the size of the map which will be used for saving the map, otherwise sets it to empty string
	void	SetMapFileSize(const std::string &fileSize);

	// returns the just the fileName from m_mapFileName, will ignore path, 
	// returns empty string if there's no file name
	std::string GetFileName();

	// reads fileSize and sets the current map to it and clears it, also clears the enemy list
	// returns false on error (wrong fileSize), writes to log
	bool	CreateNewMap();

	const std::vector<_mapPos>*	GetEnemyMapPos() const;

	void	DeleteEnemyMapPos();
};
