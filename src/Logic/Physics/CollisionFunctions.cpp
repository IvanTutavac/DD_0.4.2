#include "CollisionFunctions.h"

namespace DD
{
	bool	CheckMouseCollision(int mouseX, int mouseY, int entityX, int entityY, int entityWidth, int entityHeight)
	{
		if (mouseX > entityX && mouseX < entityX + entityWidth && mouseY > entityY && mouseY < entityY + entityHeight)
			return	true;

		return	false;
	}
}
