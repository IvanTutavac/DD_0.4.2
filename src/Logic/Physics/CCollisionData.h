#pragma once

#include <vector>

enum class CollisionType
{
	SpellEnemy
};

struct _rectData
{
	int x, y, w, h;
};

struct _rect
{
	std::vector<_rectData> rect;
};
