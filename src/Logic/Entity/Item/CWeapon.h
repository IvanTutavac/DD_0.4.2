#pragma once

enum class WeaponType;
enum class WeaponTypeEx;

class CWeapon
{
private:

	int		m_gold;
	int		m_attackValue;
	float	m_speedModifier;
	int		m_imgIndex;

	WeaponType	m_weaponType;
	WeaponTypeEx	m_weaponTypeEx;

public:

	CWeapon();
	CWeapon(int gold, int attackValue, float speedModifier, WeaponType type, WeaponTypeEx typeEx, int imgIndex);
	~CWeapon();

	bool	Init(int gold, int attackValue, float speedModifier, WeaponType type, WeaponTypeEx typeEx, int imgIndex);
	void	Clean();

	int		GetGold() const;
	int		GetAttack() const;
	float	GetSpeed() const;
	int		GetImgIndex() const;
	WeaponType	GetWeaponType() const;
	WeaponTypeEx GetWeaponTypeEx() const;
};
