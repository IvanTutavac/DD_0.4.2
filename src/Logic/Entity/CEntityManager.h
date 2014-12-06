#pragma once

#include <map>
#include <vector>
#include <string>

class CEntity;
class CPlayer;
class CAI;
class CItemManager;
class CSpellManager;

struct _entityWrapper;
struct _enemyWrapper;
struct _mapPos;
struct _mapTileAll;
struct _weaponAttackPosWrapper;
struct _leftRight;
struct _upDown;
struct _inGameHoverData;

enum class EnemyType;
enum class KeyboardEvents;
enum class InGameHoverType;

class CEntityManager
{
private:

	std::map<int, EnemyType> m_indexToEnemy; // m_indexToEnemy[index] = will return an EnemyType for the index (img number)
	std::map<EnemyType, std::string> m_enemyNameMap;

	_enemyWrapper	*m_pEnemyList; // contains every enemy available
	_entityWrapper	*m_pEnemy; // enemies alive on map
	CAI				*m_pAI;

	CItemManager	*m_pItemMng;
	CPlayer			*m_pPlayer;	

	_weaponAttackPosWrapper	*m_pPlayerCurrentWeaponAttacks; // when an entity attacks, the attack data required for renderer, animation movement, and collision is stored here
	_weaponAttackPosWrapper	*m_pEnemyCurrentWeaponAttacks;
	_inGameHoverData		*m_pInGameHoverData;
	double					m_inGameHoverDataTime;
	const double			m_inGameHoverDataTimeReset = 0.2;

private:

	bool	InitEnemyIndex();
	bool	InitEnemyList();
	bool	InitEnemyNames();

	CEntity*	GetEnemyById(const int id);

	// returns false on error and writes to log
	bool	DeleteEnemy(const int index);

	// returns true if the user is hovering over an enemy and updates hover data with that entity data
	bool	UpdateHoverOverEnemy(int mouseX1, int mouseY1, int cameraX, int cameraY, int entityX, int entityY, int entityIndex);
	bool	UpdateHoverOverHudSpell(const CSpellManager *SpellMng, int mouseX1, int mouseY1);
	bool	UpdateHoverOverHudWeapAtt(int mouseX1, int mouseY1);
	void	EmptyHoverData();

public:

	CEntityManager();
	~CEntityManager();

	bool	Init(const _mapTileAll *map, bool floorPassable[]);
	void	Clean();

	void	Update(double deltaTime);

	bool	UpdateAI(int playerX, int playerY);

	void	CreateEnemies(const std::vector<_mapPos> *enemies);

	const std::vector<_mapPos>*	GetEnemyMapPosC() const;
	std::vector<_mapPos>&	GetEnemyMapPos();

	CEntity* GetEnemy(int index);

	// returns false on error and writes to log
	bool	EnemyDied(const int index);

	void	ProcessPlayerAttack(const KeyboardEvents &key, const _leftRight &lR, const _upDown &uD, float posX, float posY);
	void	PlayerAttackHit(const int attackIndex, const int enemyIndex);
	void	DeleteCurrentWeaponAttack(const int index);
	// attackHasExpired -> pass true after the weapon attack time has expired, otherwise false
	void	DeleteCurrentPlayerWeaponAttack(int	index, bool attackHasExpired = false);

	void	UpdateInGameHoverData(int mouseX1, int mouseY1, int cameraX, int cameraY, const CSpellManager *SpellMng);

	CItemManager*	GetItemManager();
	CPlayer*		GetPlayer();
	CAI*			GetAI();
	_weaponAttackPosWrapper*	GetCurrentWeaponAttacksForPlayer();
	const _inGameHoverData*	GetInGameHoverData() const;
};
