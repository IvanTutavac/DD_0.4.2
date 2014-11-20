#pragma once

#include "SpellData.h"

class CSpell
{
private:

	int		m_imgIndex;

	int		m_value;
	int		m_cost;
	double	m_duration;
	double  m_cooldown; 
	int		m_speed;
	SpellType m_type;
	SpellList m_spell;

	_spellCaster	m_parent; // entity who casted the spell

public:

	CSpell();
	CSpell(int imgIndex, int value, int cost, double  duration,double cooldown,int speed, SpellType type, SpellList spell);
	~CSpell();

	void	SetCaster(const _spellCaster &spellCaster);

	// returns true if the spell can not live any longer, false otherwise
	bool	ReduceDuration(double deltaTime);

	double	GetCooldown() const;
	int		GetManaCost() const;
	int		GetSpeed() const;
	int		GetValue() const;
	int		GetImgIndex() const;

	SpellType	GetSpellType() const;
	SpellList	GetSpell() const;
};