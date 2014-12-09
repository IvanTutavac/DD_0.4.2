#include "CCollision.h"
#include "../Map/MapData.h"
#include "../Entity/EntityWrapper.h"
#include "../Entity/Item/ItemEnums.h"
#include "../../IO/IOFunctions.h"
#include "../../Tools/Log.h"

CCollision::CCollision()
{

}

CCollision::~CCollision()
{

}

bool	CCollision::Init()
{
	if (!DD::IO::LoadCollisionRects(m_spellRects, "spells.txt"))
	{
		Log("CCollisionInit failed, DD::IO::LoadCollisionRects for spells failed");
		return	false;
	}

	if (!DD::IO::LoadCollisionRects(m_enemyRects, "enemy.txt"))
	{
		Log("CCollision Init failed, DD::IO::LoadCollisionRects for enemies failed");
		return	false;
	}

	if (!DD::IO::LoadCollisionRects(m_itemRects, "items.txt"))
	{
		Log("CCollision Init failed, DD::IO::LoadCollisionRects for items failed");
		return	false;
	}

	if (!DD::IO::LoadCollisionRects(m_playerRects, "player.txt"))
	{
		Log("CCollision Init failed, DD::IO::LoadCollisionRects for player failed");
		return	false;
	}

	return	true;
}

void	CCollision::Clean()
{

}

void	CCollision::CheckEntityCollision(const std::vector<_mapPos> *entity1, const std::vector<_mapPos> *entity2, std::vector<std::pair<int, int>> &collided, CollisionType type, bool useRects)
{
	int i{ 0 }, j{ 0 };

	if (useRects)
	{
		for (const auto& e1 : *entity1)
		{
			for (const auto &e2 : *entity2)
			{
				if (CheckCollision((int)e1.x, (int)e1.y, (int)e2.x, (int)e2.y))
				{
					if (CheckCollisionEx(e1.index, e2.index, (int)e1.x, (int)e1.y, (int)e2.x, (int)e2.y, type)) // second, more precise checking
					{
						std::pair<int, int> found{ i, j };

						collided.push_back(found);
					}
				}

				j++;
			}

			i++;
			j = 0;
		}
	}
	else
	{
		for (const auto& e1 : *entity1)
		{
			for (const auto &e2 : *entity2)
			{
				if (CheckCollision((int)e1.x, (int)e1.y, (int)e2.x, (int)e2.y))
				{
					std::pair<int, int> found{ i, j };

					collided.push_back(found);
				}

				j++;
			}

			i++;
			j = 0;
		}
	}
}

void	CCollision::CheckWeaponAttackCollision(_weaponAttackPosWrapper *weaponAttack, const std::vector<_mapPos> *entity, std::vector<std::pair<int, int>> &collided, WeaponAttCollType type)
{
	std::vector<_rect>	*pRect = nullptr;

	if (type == WeaponAttCollType::EnemyAttOnPlayer)
	{
		pRect = &m_playerRects;
	}
	else if (type == WeaponAttCollType::PlayerAttOnEnemy)
	{
		pRect = &m_enemyRects;
	}

	int i{ 0 }, j{ 0 };

	for (auto& attack : weaponAttack->pos)
	{
		if (attack.attackType == WeaponAttackType::normalSlash)
		{
			float tX = 0, tY = 0, tX2, tY2;

			GetMapPos(tX, tY, tX2, tY2, attack.posX, attack.posY, 31, 31, attack.currentValue);

			for (const auto& e : *entity) 
			{
				if (!attack.IsAlreadyCollided(e.id) && tX2 >= e.x && tX <= e.x + 32 && tY2 >= e.y && tY <= e.y + 32) // weak check
				{
					if (WeaponEntityCollisionCheck(attack, e, *pRect)) // detailed check, will check weapon and enemy rects for collision
					{
						std::pair<int, int> found{ i, e.id };//j };

						collided.push_back(found);

						attack.collidedId.emplace_back(e.id);
					}
				}

				//j++;
			}
		}

		i++;
		j = 0;
	}
}

bool	CCollision::CheckCollision(int x1, int y1, int x2, int y2, int w2, int h2, int w1, int h1)
{
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2)
		return	true;

	return	false;
}

bool	CCollision::CheckCollisionEx(int index1, int index2, int x1, int y1, int x2, int y2, CollisionType type)
{
	std::vector<_rectData>	*pRect1 = nullptr;
	std::vector<_rectData>	*pRect2 = nullptr;

	if (type == CollisionType::SpellEnemy)
	{
		pRect1 = &m_spellRects[index1].rect;
		pRect2 = &m_enemyRects[index2].rect;
	}
	else if (type == CollisionType::PlayerEnemy)
	{
		pRect1 = &m_playerRects[index1].rect;
		pRect2 = &m_enemyRects[index2].rect;
	}

	for (const auto& rect1 : *pRect1)
	{
		int realX1 = rect1.x + x1;
		int realY1 = rect1.y + y1;

		for (const auto& rect2 : *pRect2)
		{
			int realX2 = rect2.x + x2;
			int realY2 = rect2.y + y2;

			if (CheckCollision(realX1, realY1, realX2, realY2, rect2.w, rect2.h, rect1.w, rect1.h))
				return	true;
		}
	}

	return	false;
}

void	CCollision::GetMapPos(float &x, float &y, float &endX, float &endY, float mapX, float mapY, float w, float h, float angle)
{
	angle = angle * -1 + 360;

	if (angle > 360)
		angle -= 360;

	float tX_ = x - 15, tY_ = y - 15;

	float tX = tX_ * cosf(angle) + tY_ * sinf(angle);
	float tY = -1 * tX_ * sinf(angle) + tY_ * cosf(angle);

	tX += 15 + mapX, tY += 15 + mapY; // startX,Y

	float tX2_ = w - 15, tY2_ = h - 15;

	float tX2 = tX2_ * cosf(angle) + tY2_ * sinf(angle);
	float tY2 = -1 * tX2_ * sinf(angle) + tY2_ * cosf(angle);

	tX2 += 15 + mapX, tY2 += 15 + mapY; // endX,Y

	if (tX > tX2)
	{
		float t = tX;
		tX = tX2;
		tX2 = t;
	}

	if (tY > tY2)
	{
		float t = tY;
		tY = tY2;
		tY2 = t;
	}

	x = tX;
	y = tY;
	endX = tX2;
	endY = tY2;
}

bool CCollision::WeaponEntityCollisionCheck(const _mapPosWeaponAttack &attack, const _mapPos &entity, std::vector<_rect> &rects)
{
	float tX, tY, tX2, tY2;
	
	for (const auto& attackRect : m_itemRects[attack.imgIndex].rect)
	{
		tX = static_cast<float>(attackRect.x), tY = static_cast<float>(attackRect.y);

		GetMapPos(tX, tY, tX2, tY2, attack.posX, attack.posY, static_cast<float>(attackRect.w), static_cast<float>(attackRect.h), attack.currentValue);

		for (const auto& entityRects : rects[entity.index].rect)
		{
			float eX = entityRects.x + entity.x;
			float eY = entityRects.y + entity.y;

			if (tX2 >= eX && tX <= eX + entityRects.w && tY2 >= eY && tY <= eY + entityRects.h)
				return	true;
		}
	}

	return	false;
}
