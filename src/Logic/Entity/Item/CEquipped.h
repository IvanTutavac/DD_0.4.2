#pragma once

class CWeapon;

class CEquipped
{
private:

	CWeapon	*m_pWeapon;

public:

	CEquipped();
	~CEquipped();

	const CWeapon*	GetWeaponC() const;
	void		SetWeapon(CWeapon *weapon);
};
