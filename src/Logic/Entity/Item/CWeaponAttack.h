#pragma once

#include <vector>
#include <string>

enum class WeaponAttackType;
enum class KeyboardEvents;

struct _weaponAttackData
{
	std::string name;
	double		cooldownReset;
	double		cooldown; // if it's <= 0 one can attack
	float		speed; // how fast will the attack go
	float		value; // can be angle, duration, distance of the weapon animation -> used for finishing the attack
	float		attackModifier;
	int			abilityLevel;
	WeaponAttackType	attackType;
	KeyboardEvents		key;
};

// like spells for weapons (abilities), contains available attacks for an entity
class CWeaponAttack
{
private:

	std::vector<_weaponAttackData>	m_attack; 

	bool	m_attackStarted;

public:

	CWeaponAttack();
	~CWeaponAttack();

	void	Update(double deltaTime);

	// if nothing is returned it means there's no attack mapped to the key and there's no attack already going on
	WeaponAttackType GetMappedAttack(KeyboardEvents key);

	// doesn't check if there's already an attack with the same type

	// speed -> how fast the attack actually is, it's not attack duration, an attack finishes depending on value (e.g. when it reaches a certain angle or whatever)
	// value -> can be angle, duration, distance of the weapon animation -> used for finishing the attack 
	void	AddAttack(const std::string& name, WeaponAttackType attackType, float speed, float value, float attackModifier, double cooldown, int abilityLevel, KeyboardEvents key);
	void	AddAttack(const _weaponAttackData &data);
	void	RemoveAttack(WeaponAttackType type);

	// default 1 if type was wrong

	int		GetAbilityLevel(WeaponAttackType type) const;
	float	GetSpeed(WeaponAttackType type) const;
	float	GetValue(WeaponAttackType type) const;
	float	GetAttackModifier(WeaponAttackType type) const;
	// returns true if the attack has no cooldown
	bool	IsAttackAvailable(WeaponAttackType type) const;
	void	StartAttackTimer(WeaponAttackType type);

	_weaponAttackData*	GetWeaponAttackData(WeaponAttackType type);
	const std::vector<_weaponAttackData>*	GetWeaponAttackDataC() const;

	// if passed true will also set the cooldown for the attack
	void	SetAttackStarted(bool value);
	bool	IsAttackStarted();
};
