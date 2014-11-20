#include "CItemManager.h"
#include "CWeapon.h"
#include "CWeaponAttack.h"
#include "ItemWrapper.h"
#include "ItemEnums.h"
#include "../EntityData.h"
#include "../../../Data/Enums.h"
#include "../../../Tools/debug.h"
#include "../../../Tools/Log.h"

CItemManager::CItemManager()
{
	m_pWeapons = nullptr;
	m_pInGameHUDWeapAtt = nullptr;
}

CItemManager::~CItemManager()
{

}

bool	CItemManager::Init()
{
	m_pWeapons = DD_NEW _weaponWrapper{};
	m_pInGameHUDWeapAtt = DD_NEW _inGameHudData{};

	if (!InitWeaponTypes())
	{
		Log("CItemManager::Init() failed, InitWeaponTypes failed");
		return	false;
	}

	if (!InitWeaponImgIndex())
	{
		Log("CItemManager::Init() failed, InitWeaponImgIndex failed");
		return	false;
	}

	if (!InitWeapons())
	{
		Log("CItemManager::Init() failed, InitWeapons failed");
		return	false;
	}

	if (!InitAttackList())
	{
		Log("CItemManager::Init() failed, InitAttackList failed");
		return	true;
	}

	return	true;
}

void	CItemManager::Clean()
{
	if (m_pWeapons != nullptr)
	{
		for (auto& weapon : m_pWeapons->weapons)
		{
			weapon.second.Clean();
		}
	}

	DD_DELETE(m_pWeapons);
	DD_DELETE(m_pAttackList);
	DD_DELETE(m_pInGameHUDWeapAtt);
}

bool	CItemManager::InitWeaponTypes()
{
	m_weaponTypes[WeaponTypeEx::shortSword] = WeaponType::sword;
	m_weaponTypes[WeaponTypeEx::longSword] = WeaponType::sword;
	m_weaponTypes[WeaponTypeEx::katana] = WeaponType::sword;
	m_weaponTypes[WeaponTypeEx::shortBow] = WeaponType::bow;
	m_weaponTypes[WeaponTypeEx::longBow] = WeaponType::bow;

	return	true;
}

bool	CItemManager::InitWeaponImgIndex()
{
	m_weaponImgIndex[WeaponTypeEx::longSword] = 2;
	m_weaponImgIndex[WeaponTypeEx::longBow] = 1;

	return	true;
}

bool	CItemManager::InitWeapons()
{
	AddWeapon(10, 10, 1.f, WeaponTypeEx::longSword);
	AddWeapon(12, 10, 0.9f, WeaponTypeEx::longBow);

	return	true;
}

bool	CItemManager::InitAttackList()
{
	m_pAttackList = DD_NEW CWeaponAttack{};

	m_pAttackList->AddAttack("Normal slash", WeaponAttackType::normalSlash, 250, 180, 1.f, 1.2, 1, KeyboardEvents::Q);
	m_pAttackList->AddAttack("Stab", WeaponAttackType::stab, 50, 32, 0.9f, 1.2, 1, KeyboardEvents::W);

	m_weaponAttImgIndex[WeaponAttackType::normalSlash] = m_weaponImgIndex[WeaponTypeEx::longSword];

	m_pInGameHUDWeapAtt->index.push_back(m_weaponAttImgIndex[WeaponAttackType::normalSlash]);
	m_pInGameHUDWeapAtt->x = 100, m_pInGameHUDWeapAtt->y = 0, m_pInGameHUDWeapAtt->h = 32, m_pInGameHUDWeapAtt->w = 32, m_pInGameHUDWeapAtt->offsetX = 32;

	return	true;
}

void	CItemManager::AddWeapon(int attackValue, int gold, float speedModifier, WeaponTypeEx type)
{
	CWeapon	weapon{ gold, attackValue, speedModifier, m_weaponTypes[type], type, m_weaponImgIndex[type] };

	m_pWeapons->weapons[type] = weapon;
}

CWeapon*	CItemManager::GetWeapon(WeaponTypeEx WeaponType)
{
	return	&m_pWeapons->weapons[WeaponType];
}

bool	CItemManager::IsAttackLegal(const CWeapon *weapon, WeaponAttackType attackType)
{
	if (attackType == WeaponAttackType::nothing)
		return	false;

	WeaponType type = weapon->GetWeaponType();

	if (attackType == WeaponAttackType::arrow && type == WeaponType::bow)
		return	true;
	else if (attackType == WeaponAttackType::normalSlash && (type == WeaponType::sword || type == WeaponType::axe))
		return	true;
	else if (attackType == WeaponAttackType::stab && (type == WeaponType::sword || type == WeaponType::axe))
		return	true;
	else if (attackType == WeaponAttackType::tripleArrow && type == WeaponType::bow)
		return	true;

	return	false;
}

CWeaponAttack*	CItemManager::GetAvailableAttacks()
{
	return	m_pAttackList;
}

const _weaponAttackData* CItemManager::GetWeapAttC(int weapAttImgIndex) const
{
	WeaponAttackType	attType{ WeaponAttackType::nothing };

	for (const auto& tmp : m_weaponAttImgIndex)
	{
		if (tmp.second == weapAttImgIndex)
		{
			attType = tmp.first;
			break;
		}
	}

	return	m_pAttackList->GetWeaponAttackData(attType);
}

const _inGameHudData* CItemManager::GetPlayerHUDWeapAtt() const
{
	return	m_pInGameHUDWeapAtt;
}
