#pragma once

#include "CEntity.h"
#include "EntityData.h"
#include "../Map/MapData.h"

#include <vector>
#include <map>

struct _enemyWrapper
{
	std::map<EnemyType, CEntity> entity;
};

struct _entityWrapper
{
	std::vector<CEntity> entity;
	std::vector<_mapPos> pos;
};

struct _weaponAttackPosWrapper
{
	std::vector<_mapPosWeaponAttack>	pos;
};
