#include "CWeaponAttack.h"
#include "ItemEnums.h"

CWeaponAttack::CWeaponAttack()
{
	m_attackStarted = false;
}

CWeaponAttack::~CWeaponAttack()
{

}

WeaponAttackType CWeaponAttack::GetMappedAttack(KeyboardEvents key)
{
	for (const auto& attack : m_attack)
	{
		if (attack.key == key && !m_attackStarted)
			return	attack.attackType;
	}

	return	WeaponAttackType::nothing;
}

void	CWeaponAttack::AddAttack(const std::string& name, WeaponAttackType attackType, float speed, float value, float attackModifier, double cooldown, int abilityLevel, KeyboardEvents key)
{
	m_attack.push_back(_weaponAttackData{});

	auto &t = m_attack.back();
	
	t.abilityLevel = abilityLevel;
	t.speed = speed;
	t.value = value;
	t.attackType = attackType;
	t.attackModifier = attackModifier;
	t.key = key;
	t.cooldownReset = cooldown;
	t.cooldown = 0;
	t.name = name;
}

void	CWeaponAttack::AddAttack(const _weaponAttackData &data)
{
	m_attack.push_back(data);
}

void	CWeaponAttack::RemoveAttack(WeaponAttackType type)
{
	for (int i = static_cast<int>(m_attack.size() - 1); i >= 0; i--)
	{
		if (m_attack[i].attackType == type)
		{
			m_attack.erase(m_attack.begin() + i);
		}
	}
}

void	CWeaponAttack::Update(double deltaTime)
{
	for (auto& attack : m_attack)
	{
		if (attack.cooldown > 0)
			attack.cooldown -= deltaTime;
	}
}

int		CWeaponAttack::GetAbilityLevel(WeaponAttackType type) const
{
	for (const auto& attack : m_attack)
	{
		if (attack.attackType == type)
			return	attack.abilityLevel;
	}

	return	1;
}

float		CWeaponAttack::GetSpeed(WeaponAttackType type) const
{
	for (const auto& attack : m_attack)
	{
		if (attack.attackType == type)
			return	attack.speed;
	}

	return	1;
}

float	CWeaponAttack::GetValue(WeaponAttackType type) const
{
	for (const auto& attack : m_attack)
	{
		if (attack.attackType == type)
			return	attack.value;
	}

	return	1;
}

float	CWeaponAttack::GetAttackModifier(WeaponAttackType type) const
{
	for (const auto& attack : m_attack)
	{
		if (attack.attackType == type)
			return	attack.attackModifier;
	}

	return	1;
}

bool	CWeaponAttack::IsAttackAvailable(WeaponAttackType type) const
{
	for (const auto& attack : m_attack)
	{
		if (attack.attackType == type)
		{
			if (attack.cooldown <= 0)
				return	true;
		}
	}

	return	false;
}

void	CWeaponAttack::StartAttackTimer(WeaponAttackType type)
{
	_weaponAttackData *attack{ GetWeaponAttackData(type) };

	attack->cooldown = attack->cooldownReset;
}

_weaponAttackData*	CWeaponAttack::GetWeaponAttackData(WeaponAttackType type)
{
	for (auto &attack : m_attack)
	{
		if (attack.attackType == type)
			return	&attack;
	}

	return	nullptr;
}

const std::vector<_weaponAttackData>*	CWeaponAttack::GetWeaponAttackDataC() const
{
	return	&m_attack;
}

void	CWeaponAttack::SetAttackStarted(bool value)
{
	m_attackStarted = value;
}

bool	CWeaponAttack::IsAttackStarted()
{
	return	m_attackStarted;
}
