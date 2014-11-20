#pragma once

#include "CSpell.h"
#include "../Map/MapData.h"

#include <vector>

enum class SpellList;

struct _spellWrapperExt
{
	std::vector<_mapPos>	position;
	std::vector<CSpell>	spell;
};

struct _spellWrapper
{
	std::map<SpellList,CSpell> spell;
};
