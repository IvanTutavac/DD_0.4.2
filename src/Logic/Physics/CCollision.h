#pragma once

#include "CCollisionData.h"

#include <utility>

struct _mapPos;
struct _weaponAttackPosWrapper;
struct _mapPosWeaponAttack;

class CCollision
{
private:

	std::vector<_rect>	m_spellRects;
	std::vector<_rect>	m_enemyRects;
	std::vector<_rect>	m_itemRects;

	// searches every collision rect for both entities 
	// returns true if there's a collision
	// index1 and index2 -> index of an entity, the same as imgIndex
	// x1 y1 first entity map position, x2, y2 second entity map position
	bool	CheckCollisionEx(int index1, int index2, int x1, int y1, int x2, int y2, CollisionType type);

	// updates x, y, endX, endY with the map position of the rotated entity
	// x, y pass 0 if the whole image is being used, pass a value between 0 and 31 for rects
	// endX and endY will be updated, doesn't matter what one passes because the value will not be used inside the function
	// mapX, mapY -> entity position on the map
	// w,h -> entity width and height, pass 31 for the whole image or rect.w, rect.h if rects are being used
	void	GetMapPos(float &x, float &y, float &endX, float &endY, float mapX, float mapY, float w, float h, float angle);

	// collision check between an weapon attack and entities
	// checks each collision rect, if there's a collision returns true
	// rects -> which entity to check collision for, enemy, player
	bool	WeaponEntityCollisionCheck(const _mapPosWeaponAttack &attack, const _mapPos &entity, std::vector<_rect> &rects);

public:
	CCollision();
	~CCollision();

	bool	Init();
	void	Clean();

	static bool	CheckCollision(int x1, int y1, int x2, int y2, int w2 = 32, int h2 = 32, int w1 = 32, int h1 = 32);

	// checks for collision between entity1 and entity2
	// if a collision has been found, the index of those entities will be put in collided
	void	CheckEntityCollision(const std::vector<_mapPos> *entity1, const std::vector<_mapPos> *entity2, std::vector<std::pair<int, int>> &collided, CollisionType  type);
	void	CheckWeaponAttackCollision(_weaponAttackPosWrapper *weaponAttack, const std::vector<_mapPos> *entity, std::vector<std::pair<int, int>> &collided);
};
