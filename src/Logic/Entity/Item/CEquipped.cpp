#include "CEquipped.h"

CEquipped::CEquipped()
{

}

CEquipped::~CEquipped()
{

}

const CWeapon*	CEquipped::GetWeaponC() const
{
	return	m_pWeapon;
}

void	CEquipped::SetWeapon(CWeapon *weapon)
{
	m_pWeapon = weapon;
}
