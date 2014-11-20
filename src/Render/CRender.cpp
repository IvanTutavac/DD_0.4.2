#include "CRender.h"
#include "../Data/Globals.h"

CRender::CRender()
{

}

void	CRender::AlignmentX(TextFlag::TextFlag flag, int &x, int x1, int width, int x2)
{
	if (flag & TextFlag::alignLeft)
	{
		x = x1;
	}
	else if (flag & TextFlag::alignRight)
	{
		x = x1 - width;
	}
	else if (flag & TextFlag::center)
	{
		x = (x2 - x1)/2 - (width / 2);
	}
}

bool	CRender::CheckIfInWindow(int cameraX, int cameraY, int mapWidth, int mapHeight,int &x, int &y)
{
	// get window locations
	x = x - cameraX + g_windowWidth / 2;
	y = y - cameraY + g_windowHeight / 2;

	// do not allow the entity to be drawn outside of map
	if (cameraX <= g_windowWidth / 2)
	{
		if (x < (cameraX - g_windowWidth / 2))
			return false;
	}

	if (cameraY <= g_windowHeight / 2)
	{
		if (y < (cameraY - g_windowHeight / 2))
			return	false;
	}

	if (cameraX >= (mapWidth - g_windowWidth / 2))
	{
		if (x >(cameraX + g_windowWidth / 2))
			return	false;
	}

	if (cameraY >= (mapHeight - g_windowHeight / 2))
	{
		if (y > (cameraY + g_windowHeight / 2))
			return	false;
	}

	// entity in window, let it be drawn

	return	true;
}
