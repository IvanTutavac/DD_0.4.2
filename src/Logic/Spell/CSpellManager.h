#pragma once

#include <map>
#include <string>
#include <vector>

struct _spellWrapper;
struct _spellWrapperExt; // contains struct for map positions
struct _spellCaster;
struct _mapPos;
struct _inGameHudData;

enum class SpellList;
enum class KeyboardEvents;

class CSpell;

struct spellsAvailable
{
	double	cooldown; // if cooldown <= 0 spell can be cast
	double	cooldownReset; // used to set cooldown with it's value if the spell has been casted
	SpellList	spell;
};

class CSpellManager
{
private:

	std::map<SpellList, int> m_spellMap; // contains img index for every spell
	std::map<SpellList, std::string> m_spellSNameMap; // maps a name for each spell

	_spellWrapper	*m_pSpellList; // all spells available, used to construct casted spells

	_spellWrapperExt *m_pSpellCasted; 

	std::vector<spellsAvailable>	m_playersSpells;
	std::map<KeyboardEvents, SpellList>	m_playerSpellKeyMap; // maps a spell to a key

	_inGameHudData	*m_pInGameHUDSpelldata;

	void	InitSpellMap();
	void	InitSpellList();
	void	InitPlayerSpells();

	spellsAvailable*	GetPlayerSpellAvailable(const SpellList &spell);

	void	SetSpellCooldown(const SpellList &spell, const _spellCaster &caster);

	// returns false on error and writes to log
	bool	RemoveSpell(const int index);

public:

	CSpellManager();
	~CSpellManager();

	bool	Init();
	void	Clean();

	// reduces cooldowns and spell duration
	// returns false on error and writes to log
	bool	Update(double deltaTime);

	SpellList	GetPlayerSpell(const KeyboardEvents &key);

	bool	CanCast(const _spellCaster &caster, const SpellList &spell, const int mp);

	// call CanCast before calling CastSpell so that one can construct pos
	// mp -> mana points, will be updated 
	// returns false on error and writes to log
	bool	CastSpell(const SpellList &spell, const _spellCaster &caster, const _mapPos &pos, int &mp);

	void	SpellHit(int &hp, int &mp, int &speed, int spellIndex);

	// returns img index for the spell
	int			GetSpellIndex(const SpellList &spell);

	const CSpell*	GetSpellC(const SpellList &spell) const;

	const CSpell*	GetSpellC(int imgIndex) const;

	std::vector<_mapPos>*	GetSpellMapPos();

	void		DeleteSpells(const std::vector<int> &index);

	const _inGameHudData*	GetPlayerHudSpells()  const;

	const std::string&	GetSpellName(SpellList spell) const;
};
