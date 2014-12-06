#pragma once

#include <vector>

struct _mapPos;
struct _weaponAttackPosWrapper;
struct _leftRight;
struct _upDown;

class CMovement
{
private:

	void	MoveRight(float &x, float speed, double deltaTime, int mapWidth);
	void	MoveLeft(float &x, float speed, double deltaTime);
	void	MoveUp(float &y, float speed, double deltaTime);
	void	MoveDown(float &y, float speed, double deltaTime, int mapHeight);

public:

	CMovement();
	~CMovement();

	bool	Init();
	void	Clean();

	// move camera, checks included
	void	UpdateCamera(const int mouseX, const int mouseY, float &cameraX, float &cameraY,const int mapWidth, const int mapHeight, const double deltaTime) const;
	
	void	UpdateMapMovePosition(_mapPos &pos, float mouseX, float mouseY, float cameraX, float cameraY);

	// cleanIndex will be filled with indexes of the spells which need to be cleaned
	void	MoveSpells(std::vector<_mapPos> *position, double deltaTime, int mapWidth, int mapHeight, std::vector<int> &cleanIndex);

	void	MoveEntity(const _leftRight &leftRight, const _upDown &upDown, float &x, float &y, float speed, double deltaTime, int mapWidth, int mapHeight);

	bool	MoveEntity(_mapPos	&pos, double deltaTime, int mapWidth, int mapHeight, float diff = 1);

	void	MoveEntity(_mapPos &pos, std::vector<std::pair<int, int>> &path, double deltaTime, float diff = 1);

	// cleanIndex will be filled with indexes of the attacks which need to be cleaned
	void	MoveWeaponAttacks(_weaponAttackPosWrapper *pos, double deltaTime, int mapWidth, int mapHeight, std::vector<int> &cleanIndex);
};
