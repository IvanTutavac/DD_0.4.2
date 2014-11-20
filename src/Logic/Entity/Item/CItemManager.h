#pragma once

#include <map>

class CWeapon;
class CWeaponAttack;

struct _weaponWrapper;
struct _inGameHudData;
struct _weaponAttackData;

enum class WeaponType;
enum class WeaponTypeEx;
enum class WeaponAttackType;

class CItemManager
{
private:

	CWeaponAttack	*m_pAttackList; // weapon attacks available, used to construct entity attacks
	_weaponWrapper	*m_pWeapons; // all available weapons, accessed by WeaponTypeEx (std::map)

	_inGameHudData	*m_pInGameHUDWeapAtt;

	std::map<WeaponTypeEx, int>	m_weaponImgIndex; // returns item img index for weapons
	std::map<WeaponTypeEx, WeaponType> m_weaponTypes; // returns weapon type for each weapon
	std::map<WeaponAttackType, int> m_weaponAttImgIndex; // contains item img index for weapon attacks

	bool	InitWeaponTypes();
	bool	InitWeaponImgIndex();
	bool	InitWeapons();
	bool	InitAttackList();
	
	void	AddWeapon(int attackValue, int gold, float speedModifier, WeaponTypeEx type);

public:

	CItemManager();
	~CItemManager();

	bool	Init();
	void	Clean();

	CWeapon*	GetWeapon(WeaponTypeEx WeaponType);

	// checks whether the attack type is legal for the weapon, if yes returns true
	bool	IsAttackLegal(const CWeapon *weapon, WeaponAttackType attackType);

	CWeaponAttack*	GetAvailableAttacks();
	const _weaponAttackData* GetWeapAttC(int weapAttImgIndex) const;

	const _inGameHudData *GetPlayerHUDWeapAtt() const;
};
