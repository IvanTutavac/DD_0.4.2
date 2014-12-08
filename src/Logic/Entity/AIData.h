#pragma once

#include <vector>

enum class AIEntityState
{
	nothing, moving, reached, attacking
};

struct _aiData
{
	std::vector<std::pair<int, int>>	path;
	int	playerX, playerY;
	AIEntityState	state;
	bool	movementDisabled; // if true entity can not move
};
