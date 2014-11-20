#include "CSpell.h"
#include "../../Data/Globals.h"

CSpell::CSpell()
{

}

CSpell::CSpell(int imgIndex, int value, int cost, double  duration,double cooldown, int speed, SpellType type, SpellList spell)
{
	m_imgIndex = imgIndex;
	m_value = value;
	m_cost = cost;
	m_duration = duration;
	m_cooldown = cooldown;
	m_speed = speed;
	m_type = type;
	m_spell = spell;
}

CSpell::~CSpell()
{

}

void	CSpell::SetCaster(const _spellCaster &spellCaster)
{
	m_parent = spellCaster;
}

bool	CSpell::ReduceDuration(double deltaTime)
{
	m_duration -= deltaTime;

	if (m_duration < 0)
	{
		return	true;
	}

	return	false;
}

double	CSpell::GetCooldown() const
{
	return	m_cooldown;
}

int		CSpell::GetManaCost() const
{
	return	m_cost;
}

int		CSpell::GetSpeed() const
{
	return	m_speed;
}

int		CSpell::GetValue() const
{
	return	m_value;
}

SpellType	CSpell::GetSpellType() const
{
	return	m_type;
}

int		CSpell::GetImgIndex() const
{
	return	m_imgIndex;
}

SpellList	CSpell::GetSpell() const
{
	return	m_spell;
}
