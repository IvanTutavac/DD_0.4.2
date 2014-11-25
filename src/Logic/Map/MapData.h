#pragma once

#include <vector>

#include "../../Data/Const.h"

enum class MapType
{
	nothing, small_, medium, big, overworld
};

enum class MapEmpty
{
	nothing, everything, img, entity
};

struct _mapPos
{
	float	x, y;
	float	movX, movY; // used for movement
	float	endX, endY;
	int		index; // img index, used to access the image, type known from the variable (for example, MapPos Enemy; Enemy -> img of type monster)
	int		id; // entity id, to know which entity is which image
	int		speed;
};


struct _mapTile
{
	int imgIndex;
	int	entityIndex;
};

struct _mapTileSmall
{
	_mapTile	map[MAP_HEIGHT_SMALL / 32][MAP_WIDTH_SMALL / 32];
};

struct _mapTileMedium
{
	_mapTile	map[MAP_HEIGHT_MEDIUM / 32][MAP_WIDTH_MEDIUM / 32];
};

struct _mapTileBig
{
	_mapTile	map[MAP_HEIGHT_BIG / 32][MAP_WIDTH_BIG / 32];
};

struct _mapTileAll
{
	MapType			*currentMap;
	_mapTileSmall	*small_;
	_mapTileMedium	*medium;
	_mapTileBig		*big;
};

struct _mapEditorImg
{
	int			map[MAP_HEIGHT_SMALL / 32][MAP_WIDTH_SMALL / 32];
};
