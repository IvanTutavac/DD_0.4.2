#pragma once

#include <vector>
#include <string>

enum class EntityType
{
	nothing, player, monster, npc
};

enum class EnemyType
{
	nothing, imp, skeleton
};

enum class WeaponAttackType;
enum class WeaponAttackDirection;

struct _mapPosWeaponAttack
{
	std::vector<int>	collidedId; 
	WeaponAttackType	attackType;
	WeaponAttackDirection	direction;
	EntityType			entityType;
	EnemyType			enemyType;
	float	posX, posY;
	float	value; // used for whatever the attack requires (angle, distance, duration ...)
	float	currentValue;
	float	speed;
	int		entityId;
	int		imgIndex;
	bool	deleteAfterCollision; // if set to true will be deleted after a collision has been detected

	// returns false if there's no collision with such entity
	bool	IsAlreadyCollided(int entityID) const
	{
		for (size_t i = 0; i < collidedId.size(); ++i)
		{
			if (collidedId[i] == entityID)
				return	true;
		}

		return	false;
	}
};

enum class InGameHoverType
{
	nothing, enemy, hudSpell, hudWeapAtt
};

struct _inGameHoverData
{
	InGameHoverType	inGameHoverType; 
	int				inGameHoverIndex; 
	int				posX, posY;
	int				w, h;
	std::vector<std::string>	data;
};

struct _inGameHudData
{
	std::vector<int>	index;
	int x, y, w, h;  // positions of the first spell on the hud not the window
	int offsetX; // startingX of the second spell = x + w + offsetX
};
