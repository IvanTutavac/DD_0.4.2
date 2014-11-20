#include "CWeapon.h"
#include "ItemEnums.h"

CWeapon::CWeapon()
{
	m_attackValue = 0;
	m_gold = 0;
	m_speedModifier = 1;
	m_weaponType = WeaponType::nothing;
	m_weaponTypeEx = WeaponTypeEx::nothing;
	m_imgIndex = -1;
}

CWeapon::CWeapon(int gold, int attackValue, float speedModifier, WeaponType type, WeaponTypeEx typeEx, int imgIndex)
{
	m_gold = gold, m_attackValue = attackValue, m_speedModifier = speedModifier, m_weaponType = type, m_weaponTypeEx = typeEx, m_imgIndex = imgIndex;
}

CWeapon::~CWeapon()
{

}

bool	CWeapon::Init(int gold, int attackValue, float speedModifier, WeaponType type, WeaponTypeEx typeEx, int imgIndex)
{
	m_gold = gold, m_attackValue = attackValue, m_speedModifier = speedModifier, m_weaponType = type, m_weaponTypeEx = typeEx, m_imgIndex = imgIndex;

	return	true;
}

void	CWeapon::Clean()
{

}

int		CWeapon::GetAttack() const
{
	return	m_attackValue;
}

int		CWeapon::GetGold() const
{
	return	m_gold;
}

float	CWeapon::GetSpeed() const
{
	return	m_speedModifier;
}

int		CWeapon::GetImgIndex() const
{
	return	m_imgIndex;
}

WeaponType	CWeapon::GetWeaponType() const
{
	return	m_weaponType;
}

WeaponTypeEx	CWeapon::GetWeaponTypeEx() const
{
	return	m_weaponTypeEx;
}
