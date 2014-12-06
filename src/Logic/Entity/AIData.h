#pragma once

#include <vector>

enum class AIMovement
{
	nothing, moving, reached
};

struct _aiData
{
	std::vector<std::pair<int, int>>	path;
	int	playerX, playerY;
	AIMovement	movement;
};
