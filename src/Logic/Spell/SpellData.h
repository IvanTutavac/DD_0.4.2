#pragma once

#include <vector>

enum class SpellList
{
	nothing,
	fireBall,
	iceLance
};

enum class SpellType
{
	damage
};

enum class EntityType;

struct _spellCaster
{
	EntityType	type;
	int		id;
};
