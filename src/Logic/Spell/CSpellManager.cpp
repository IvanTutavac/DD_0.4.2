#include "CSpellManager.h"
#include "SpellWrapper.h"
#include "../Entity/EntityData.h"
#include "../../Data/Enums.h"
#include "../../Tools/debug.h"
#include "../../Tools/Log.h"

CSpellManager::CSpellManager()
{
	m_pSpellCasted = nullptr;
	m_pSpellList = nullptr;
	m_pInGameHUDSpelldata = nullptr;
}

CSpellManager::~CSpellManager()
{

}

bool	CSpellManager::Init()
{
	m_pSpellList = DD_NEW _spellWrapper();
	m_pSpellCasted = DD_NEW _spellWrapperExt();
	m_pInGameHUDSpelldata = DD_NEW _inGameHudData{};

	InitSpellMap();
	InitSpellList();
	InitPlayerSpells();

	return	true;
}

void	CSpellManager::Clean()
{
	DD_DELETE(m_pSpellList);
	DD_DELETE(m_pSpellCasted);
	DD_DELETE(m_pInGameHUDSpelldata);
}

void	CSpellManager::InitSpellMap()
{
	m_spellMap[SpellList::iceLance] = 0;
	m_spellMap[SpellList::fireBall] = 1;

	m_spellSNameMap[SpellList::iceLance] = "Ice lance";
	m_spellSNameMap[SpellList::fireBall] = "Fireball";
}

void	CSpellManager::InitSpellList()
{
	CSpell	fireBall{ m_spellMap[SpellList::fireBall], 30, 10, 2, 1, 350, SpellType::damage, SpellList::fireBall };
	CSpell	iceLance{ m_spellMap[SpellList::iceLance], 40, 10, 3, 2, 250, SpellType::damage, SpellList::iceLance };

	m_pSpellList->spell[SpellList::fireBall] = fireBall;
	m_pSpellList->spell[SpellList::iceLance] = iceLance;

}

void	CSpellManager::InitPlayerSpells()
{
	spellsAvailable	fireBall{};

	fireBall.cooldown = 0;
	fireBall.cooldownReset = m_pSpellList->spell[SpellList::fireBall].GetCooldown();
	fireBall.spell = SpellList::fireBall;

	m_playersSpells.push_back(fireBall);

	spellsAvailable	iceLance{};

	iceLance.cooldown = 0;
	iceLance.cooldownReset = m_pSpellList->spell[SpellList::iceLance].GetCooldown();
	iceLance.spell = SpellList::iceLance;

	m_playersSpells.push_back(iceLance);

	m_playerSpellKeyMap[KeyboardEvents::one] = SpellList::fireBall;
	m_playerSpellKeyMap[KeyboardEvents::two] = SpellList::iceLance;

	m_pInGameHUDSpelldata->index.push_back(m_spellMap[m_playerSpellKeyMap[KeyboardEvents::one]]);
	m_pInGameHUDSpelldata->index.push_back(m_spellMap[m_playerSpellKeyMap[KeyboardEvents::two]]);
	m_pInGameHUDSpelldata->x = 100, m_pInGameHUDSpelldata->y = 0, m_pInGameHUDSpelldata->h = 32, m_pInGameHUDSpelldata->w = 32, m_pInGameHUDSpelldata->offsetX = 32;
}

bool	CSpellManager::CastSpell(const SpellList &spell, const _spellCaster &caster, const _mapPos &pos, int &mp)
{
	if (spell == SpellList::nothing)
	{
		Log("CSpellManager::CastSpell failed, wrong spell");
		return	false;
	}

	m_pSpellCasted->spell.push_back(CSpell{ m_pSpellList->spell[spell] });

	m_pSpellCasted->spell.back().SetCaster(caster);

	m_pSpellCasted->position.push_back(pos);

	mp -= m_pSpellList->spell[spell].GetManaCost();

	SetSpellCooldown(spell, caster);

	return	true;
}

void	CSpellManager::SpellHit(int &hp, int &mp, int &speed, int spellIndex)
{
	const CSpell *Spell = &m_pSpellCasted->spell[spellIndex];

	switch (Spell->GetSpellType())
	{
	case SpellType::damage:
	{
		hp -= Spell->GetValue();
		break;
	}
	default:
		break;
	}
}

bool	CSpellManager::Update(double deltaTime)
{
	for (int i = static_cast<int>(m_pSpellCasted->spell.size() - 1); i >= 0; i--)
	{
		if (m_pSpellCasted->spell[i].ReduceDuration(deltaTime))
		{
			if (!RemoveSpell(i))
			{
				Log("CSpellManager::Update failed");
				return	false;
			}
		}
	}

	for (auto& spell : m_playersSpells)
	{
		if (spell.cooldown > 0)
		{
			spell.cooldown -= deltaTime;
		}
	}

	return	true;
}

bool	CSpellManager::RemoveSpell(const int index)
{
	if (index < 0 || index >= static_cast<int>(m_pSpellCasted->spell.size()) || index >= static_cast<int>(m_pSpellCasted->position.size()))
	{
		Log("CSpellManager::RemoveSpell failed, wrong index: ", index);
		return	false;
	}

	m_pSpellCasted->spell.erase(m_pSpellCasted->spell.begin() + index);
	m_pSpellCasted->position.erase(m_pSpellCasted->position.begin() + index);

	return	true;
}

SpellList	CSpellManager::GetPlayerSpell(const KeyboardEvents &key)
{
	switch (key)
	{
	case KeyboardEvents::one :
		return	m_playerSpellKeyMap[key];
		break;
	case	KeyboardEvents::two :
		return	m_playerSpellKeyMap[key];
		break;
	default:
		return	SpellList::nothing;
		break;
	}
}

bool	CSpellManager::CanCast(const _spellCaster &caster, const SpellList &spell,const int mp)
{
	if (caster.type == EntityType::player)
	{
		spellsAvailable *search = GetPlayerSpellAvailable(spell);

		if (search != nullptr && search->cooldown <= 0 && mp >= m_pSpellList->spell[spell].GetManaCost())
			return	true;
		else
			return	false;
	}

	return	false;
}

spellsAvailable*	CSpellManager::GetPlayerSpellAvailable(const SpellList &spell)
{
	for (auto& search : m_playersSpells)
	{
		if (search.spell == spell)
		{
			return	&search;
		}
	}

	return	nullptr;
}

void	CSpellManager::SetSpellCooldown(const SpellList &spell, const _spellCaster &caster)
{
	if (caster.type == EntityType::player)
	{
		spellsAvailable *search = GetPlayerSpellAvailable(spell);

		search->cooldown = search->cooldownReset;
	}
}

int		CSpellManager::GetSpellIndex(const SpellList &spell)
{
	return	m_spellMap[spell];
}

const CSpell*	CSpellManager::GetSpellC(const SpellList &spell) const
{
	return	&m_pSpellList->spell[spell];
}

const CSpell*	CSpellManager::GetSpellC(int imgIndex) const
{
	for (const auto& spell : m_pSpellList->spell)
	{
		if (spell.second.GetImgIndex() == imgIndex)
			return	&spell.second;
	}

	return	nullptr;
}

std::vector<_mapPos>*	CSpellManager::GetSpellMapPos()
{
	return	&m_pSpellCasted->position;
}

void	CSpellManager::DeleteSpells(const std::vector<int> &index)
{
	for (int i = static_cast<int>(index.size() - 1); i >= 0; i--)
	{
		if (index[i] >= 0 && index[i] < static_cast<int>(m_pSpellCasted->spell.size()))
		{
			m_pSpellCasted->spell.erase(m_pSpellCasted->spell.begin() + index[i]);
			m_pSpellCasted->position.erase(m_pSpellCasted->position.begin() + index[i]);
		}
	}
}

const _inGameHudData*	CSpellManager::GetPlayerHudSpells() const
{
	return	m_pInGameHUDSpelldata;
}

const std::string&	CSpellManager::GetSpellName(SpellList spell) const
{
	// m_spellSNameMap[spell] won't work because it will result into an insertion if the key doesn't exist
	return	m_spellSNameMap.at(spell); //.find(spell)->second; // does the same thing 
}
