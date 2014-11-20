#pragma once

#include <map>
#include <vector>

#include "CWeapon.h"
#include "../../Map/MapData.h"
#include "CWeaponAttack.h"

enum class WeaponTypeEx;

struct _weaponWrapper
{
	std::map<WeaponTypeEx, CWeapon>	weapons;
};
