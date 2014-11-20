 #include "CMovement.h"
#include "../../Data/Globals.h"
#include "../Map/MapData.h"
#include "../../Data/Enums.h"
#include "../../Input/InputData.h"
#include "../Entity/EntityWrapper.h"
#include "../Entity/Item/ItemEnums.h"

#include <cmath>

CMovement::CMovement()
{

}

CMovement::~CMovement()
{

}

bool	CMovement::Init()
{
	return	true;
}

void	CMovement::Clean()
{

}

void	CMovement::UpdateCamera(const int mouseX, const int mouseY, float &cameraX, float &cameraY,const int mapWidth,const int mapHeight, const double deltaTime) const
{
	float v = 400;
	float vDiag = v*0.70710678118f;

	if (mouseX >= g_windowWidth - 1 && cameraX < mapWidth - g_windowWidth / 2)
	{
		if (mouseY <= 0 && cameraY > g_windowHeight / 2) // top right
		{
			cameraY -= (float)(vDiag*deltaTime);
			cameraX += (float)(vDiag*deltaTime);
		}
		else if (mouseY >= g_windowHeight - 1 + g_windowHUDHeight && cameraY < mapHeight - g_windowHeight / 2) // bottom right
		{
			cameraY += (float)(vDiag*deltaTime);
			cameraX += (float)(vDiag*deltaTime);
		}
		else
		{
			cameraX += (float)(v*deltaTime); // right
		}
	}
	else if (mouseX <= 0 && cameraX > g_windowWidth / 2)
	{
		if (mouseY <= 0 && cameraY > g_windowWidth / 2) // top left
		{
			cameraY -= (float)(vDiag*deltaTime);
			cameraX -= (float)(vDiag*deltaTime);
		}
		else if (mouseY >= g_windowHeight - 1 + g_windowHUDHeight && cameraY < mapHeight - g_windowHeight / 2) // bottom left
		{
			cameraY += (float)(vDiag*deltaTime);
			cameraX -= (float)(vDiag*deltaTime);
		}
		else
		{
			cameraX -= (float)(v*deltaTime); // left
		}
	}
	else if (mouseY >= g_windowHeight - 1 + g_windowHUDHeight && cameraY < mapHeight - g_windowHeight / 2) // bottom
	{
		cameraY += (float)(v*deltaTime);
	}
	else if (mouseY <= 0 && cameraY > g_windowHeight / 2) // top
	{
		cameraY -= (float)(v*deltaTime);
	}

	if (cameraY > mapWidth - g_windowWidth / 2)
		cameraX = mapWidth - g_windowWidth / 2.f;
	else if (cameraX < g_windowWidth / 2)
		cameraX = g_windowWidth / 2.f;

	if (cameraY < g_windowHeight / 2)
		cameraY = g_windowHeight / 2.f;
	else if (cameraY > mapHeight - g_windowHeight / 2)
		cameraY = mapHeight - g_windowHeight / 2.f;
}

void	CMovement::UpdateMapMovePosition(_mapPos &pos, float mouseX, float mouseY, float cameraX, float cameraY)
{
	float x{ mouseX };
	float y{ mouseY };

	// pos.x/y are map positions, we need window positions (x/y) to calculate an unit vector
	// 16 added for centering the position, otherwise it will be the top left tile position
	x -= pos.x + 16 - cameraX + g_windowWidth / 2; 
	y -= pos.y + 16 - cameraY + g_windowHeight / 2;

	float d = sqrtf(x * x + y * y);

	x /= d;
	y /= d;

	pos.movX = x;
	pos.movY = y;

	// subtracting 16 because we want an entity to have its center on mouse positions, so its position is center - 16
	pos.endX = mouseX - 16 + cameraX - g_windowWidth / 2; // map positions
	pos.endY = mouseY - 16 + cameraY - g_windowHeight / 2;
}

void	CMovement::MoveSpells(std::vector<_mapPos> *position,double deltaTime, int mapWidth, int mapHeight, std::vector<int> &cleanIndex)
{
	int i{ 0 };

	for (auto& pos : *position)
	{
		pos.x += static_cast<float>(pos.movX * deltaTime * pos.speed);
		pos.y += static_cast<float>(pos.movY * deltaTime * pos.speed);

		// check if the spell is outside of the map
		if (pos.x < 0 || pos.x > mapWidth - 32 || pos.y < 0 || pos.y > mapHeight - 32)
		{
			cleanIndex.push_back(i);
		}

		i++;
	}
}

bool	CMovement::MoveEntity(_mapPos	&pos, double deltaTime, int mapWidth, int mapHeight, float diff)
{
	pos.x += static_cast<float>(pos.movX * deltaTime * pos.speed);
	pos.y += static_cast<float>(pos.movY * deltaTime * pos.speed);

	float diffX{ pos.endX - pos.x }, diffY{ pos.endY - pos.y };

	if (diffX > diff * -1 && diffX < diff && diffY > diff * -1 && diffY < diff)
		return	false;

	int k{ 0 };

	if (pos.x < 0)
		pos.x = 0, ++k;
	else if (pos.x > mapWidth - TILE_SIZE)
		pos.x = static_cast<float>(mapWidth - TILE_SIZE), ++k;

	if (pos.y < 0)
		pos.y = 0, ++k;
	else if (pos.y > mapHeight - TILE_SIZE)
		pos.y = static_cast<float>(mapHeight - TILE_SIZE), ++k;

	if (k != 0)
		return	false;

	return	true;
}

void	CMovement::MoveWeaponAttacks(_weaponAttackPosWrapper *pos, double deltaTime, int mapWidth, int mapHeight, std::vector<int> &cleanIndex)
{
	int i{ 0 };
	for (auto& attack : pos->pos)
	{
		if (attack.attackType == WeaponAttackType::normalSlash)
		{
			attack.currentValue += static_cast<float>(attack.speed * deltaTime);

			if (attack.currentValue >= attack.value)
			{
				cleanIndex.push_back(i);
			}
		}

		i++;
	}
}

void	CMovement::MoveEntity(const _leftRight &leftRight,const _upDown &upDown, float &x, float &y, float speed, double deltaTime, int mapWidth, int mapHeight)
{
	float k = 0.70710678118f;

	if (leftRight.leftRight == LeftRight::Right && leftRight.state == InputState::pressed)
	{
		if (upDown.upDown == UpDown::Up && upDown.state == InputState::pressed)
		{
			MoveUp(y, speed*k, deltaTime);
			MoveRight(x, speed*k, deltaTime,mapWidth);
		}
		else if (upDown.upDown == UpDown::Down && upDown.state == InputState::pressed)
		{
			MoveDown(y, speed*k, deltaTime, mapHeight);
			MoveRight(x, speed*k, deltaTime, mapWidth);
		}
		else
			MoveRight(x, speed, deltaTime, mapWidth);
	}
	else if (leftRight.leftRight == LeftRight::Left && leftRight.state == InputState::pressed)
	{
		if (upDown.upDown == UpDown::Up && upDown.state == InputState::pressed)
		{
			MoveUp(y, speed*k, deltaTime);
			MoveLeft(x, speed*k, deltaTime);
		}
		else if (upDown.upDown == UpDown::Down && upDown.state == InputState::pressed)
		{
			MoveDown(y, speed*k, deltaTime, mapHeight);
			MoveLeft(x, speed*k, deltaTime);
		}
		else
			MoveLeft(x, speed, deltaTime);
	}
	else if (upDown.upDown == UpDown::Up && upDown.state == InputState::pressed)
		MoveUp(y, speed, deltaTime);
	else if (upDown.upDown == UpDown::Down && upDown.state == InputState::pressed)
		MoveDown(y, speed, deltaTime,mapHeight);
}


void	CMovement::MoveRight(float &x, float speed, double deltaTime,int mapWidth)
{
	x += static_cast<float>(speed*deltaTime);

	if (x > mapWidth - 32)
	{
		x = static_cast<float>(mapWidth - 32);
	}
}

void	CMovement::MoveLeft(float &x, float speed, double deltaTime)
{
	x -= static_cast<float>(speed*deltaTime);

	if (x < 0)
	{
		x = 0.f;
	}
}

void	CMovement::MoveUp(float &y, float speed, double deltaTime)
{
	y -= static_cast<float>(speed*deltaTime);

	if (y < 0)
	{
		y = 0.f;
	}
}

void	CMovement::MoveDown(float &y, float speed, double deltaTime, int mapHeight)
{
	y += static_cast<float>(speed*deltaTime);

	if (y > mapHeight - 32)
	{
		y = static_cast<float>(mapHeight - 32 - 1);
	}
}

