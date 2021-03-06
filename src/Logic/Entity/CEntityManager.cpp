#include "CEntityManager.h"
#include "EntityWrapper.h"
#include "CAI.h"
#include "CPlayer.h"
#include "Item/CItemManager.h"
#include "Item/CEquipped.h"
#include "Item/CWeaponAttack.h"
#include "Item/CWeapon.h"
#include "Item/ItemEnums.h"
#include "../Spell/CSpellManager.h"
#include "../Spell/CSpell.h"
#include "../Map/MapData.h"
#include "../Physics/CollisionFunctions.h"
#include "../../Data/Enums.h"
#include "../../Data/Globals.h"
#include "../../Input/InputData.h"
#include "../../Tools/debug.h"
#include "../../Tools/Log.h"
#include "../../Tools/Functions.h"

CEntityManager::CEntityManager()
{
	m_pEnemy = nullptr;
	m_pEnemyList = nullptr;
	m_pPlayer = nullptr;
	m_pItemMng = nullptr;
	m_pPlayerCurrentWeaponAttacks = nullptr;
	m_pEnemyCurrentWeaponAttacks = nullptr;
	m_pAI = nullptr;
}

CEntityManager::~CEntityManager()
{

}

bool	CEntityManager::Init(const _mapTileAll *map, bool floorPassable[])
{
	m_pEnemyList = DD_NEW _enemyWrapper{};
	m_pEnemy = DD_NEW _entityWrapper{};
	m_pPlayer = DD_NEW CPlayer{};
	m_pItemMng = DD_NEW CItemManager{};
	m_pAI = DD_NEW CAI{ map, m_pEnemy, floorPassable };

	if (!InitEnemyIndex())
	{
		Log("CEntityManager::Init() failed, InitEnemyIndex failed");
		return	false;
	}

	if (!InitEnemyList())
	{
		Log("CEntityManager::Init() failed, InitEnemyList failed");
		return	false;
	}

	if (!InitEnemyNames())
	{
		Log("CEntityManager::Init() failed, InitEnemyNames failed");
		return	false;
	}

	if (!m_pItemMng->Init())
	{
		Log("CEntityManager::Init() failed, Item Manager Init failed");
		return	false;
	}

	if (!m_pPlayer->Init(50, 200, 100))
	{
		Log("CEntityManager::Init() failed, Player Init failed");
		return	false;
	}

	if (!m_pAI->Init())
	{
		Log("CEntityManager::Init() failed, CAI::Init caused the fail");
		return	false;
	}

	m_pPlayer->GetEntity()->GetEquipped()->SetWeapon(m_pItemMng->GetWeapon(WeaponTypeEx::longSword));
	m_pPlayer->GetEntity()->GetWeaponAttack()->AddAttack(*m_pItemMng->GetAvailableAttacks()->GetWeaponAttackData(WeaponAttackType::normalSlash));

	m_pPlayerCurrentWeaponAttacks = DD_NEW _weaponAttackPosWrapper{};
	m_pEnemyCurrentWeaponAttacks = DD_NEW _weaponAttackPosWrapper{};

	m_pInGameHoverData = DD_NEW _inGameHoverData{};
	m_pInGameHoverData->inGameHoverType = InGameHoverType::nothing;

	m_inGameHoverDataTime = m_inGameHoverDataTimeReset;

	return	true;
}

void	CEntityManager::Clean()
{
	if (m_pEnemy != nullptr)
	{
		for (auto& enemy : m_pEnemy->entity)
		{
			enemy.Clean();
		}
	}

	if (m_pEnemyList != nullptr)
	{
		for (auto & enemy : m_pEnemyList->entity)
		{
			enemy.second.Clean();
		}
	}

	if (m_pPlayer != nullptr)
		m_pPlayer->Clean();
	if (m_pItemMng != nullptr)
		m_pItemMng->Clean();
	if (m_pAI != nullptr)
		m_pAI->Clean();

	DD_DELETE(m_pEnemy);
	DD_DELETE(m_pEnemyList);
	DD_DELETE(m_pPlayer);
	DD_DELETE(m_pItemMng);
	DD_DELETE(m_pInGameHoverData);
	DD_DELETE(m_pAI);
	DD_DELETE(m_pPlayerCurrentWeaponAttacks);
	DD_DELETE(m_pEnemyCurrentWeaponAttacks);
}

bool	CEntityManager::InitEnemyIndex()
{
	m_indexToEnemy[0] = EnemyType::nothing;
	m_indexToEnemy[1] = EnemyType::imp;
	m_indexToEnemy[2] = EnemyType::skeleton;

	return	true;
}

bool	CEntityManager::InitEnemyList()
{
	CEntity	Imp{ 50, 0, 70, EnemyType::imp };

	CEntity	Skeleton{ 40, 0, 80, EnemyType::skeleton };

	m_pEnemyList->entity[EnemyType::imp] = Imp;
	m_pEnemyList->entity[EnemyType::skeleton] = Skeleton;

	return	true;
}

bool	CEntityManager::InitEnemyNames()
{
	m_enemyNameMap[EnemyType::imp] = "Imp";
	m_enemyNameMap[EnemyType::skeleton] = "Skeleton";

	return	true;
}

void	CEntityManager::Update(double deltaTime)
{
	for (auto& enemy : m_pEnemy->entity)
	{
		enemy.Update(deltaTime);
	}

	m_pPlayer->GetEntity()->Update(deltaTime);

	if (m_inGameHoverDataTime > 0)
		m_inGameHoverDataTime -= deltaTime;
}

bool	CEntityManager::UpdateAI(int playerX, int playerY)
{
	if (!m_pAI->UpdateAI(playerX, playerY))
	{
		Log("CEntityManager::UpdateAI(int,int) failed, CAI::UpdateAI(int,int) caused the fail");
		return	false;
	}

	std::vector<int>	&entitiesReadyForAttack{ m_pAI->GetEntitiesReadyForAttack() };

	if (!entitiesReadyForAttack.empty())
	{
		ProcessEnemyAttack(entitiesReadyForAttack, playerX, playerY);
	}

	return	true;
}

void	CEntityManager::CreateEnemies(const std::vector<_mapPos> *enemies)
{
	for (const auto& enemy : *enemies)
	{
		const CEntity *E = &m_pEnemyList->entity[m_indexToEnemy[enemy.index]];
		
		// this way entities construct their id by themselves
		m_pEnemy->entity.push_back(CEntity{ E->GetHp(), E->GetMp(), E->GetSpeed(), E->GetEnemyType() });
		
		m_pEnemy->pos.push_back(enemy);

		m_pEnemy->pos.back().id = m_pEnemy->entity.back().GetId(); // overwrite the old id
		m_pEnemy->pos.back().speed = E->GetSpeed();

		m_pEnemy->entity.back().GetEquipped()->SetWeapon(m_pItemMng->GetWeapon(WeaponTypeEx::longSword));
		m_pEnemy->entity.back().GetWeaponAttack()->AddAttack(*m_pItemMng->GetAvailableAttacks()->GetWeaponAttackData(WeaponAttackType::normalSlash));

		m_pAI->AddEnemy();
	}
}

const std::vector<_mapPos>*	CEntityManager::GetEnemyMapPosC() const
{
	return	&m_pEnemy->pos;
}

std::vector<_mapPos>&	CEntityManager::GetEnemyMapPos()
{
	return	m_pEnemy->pos;
}

CEntity*	CEntityManager::GetEnemy(int index)
{
	if ((int)m_pEnemy->entity.size() <= index || index < 0)
		return	nullptr;

	return	&m_pEnemy->entity[index];
}

bool	CEntityManager::DeleteEnemy(const int index)
{
	if (index < 0 || index >= static_cast<int>(m_pEnemy->pos.size()) || index >= static_cast<int>(m_pEnemy->entity.size()))
	{
		Log("CEntityManager::DeleteEnemy() failed, wrong index: ", index);
		return	false;
	}

	if (m_pInGameHoverData->inGameHoverIndex == index) // the entity who died is being hovered over, reset the data
	{
		EmptyHoverData();
	}

	if (!m_pAI->DeleteEnemy(index))
	{
		Log("CEntityManager::DeleteEnemy() failed, CAI::DeleteEnemy(int) caused the fail");
		return	false;
	}

	int enemyID{ m_pEnemy->entity[index].GetId() };

	for (int i = m_pEnemyCurrentWeaponAttacks->pos.size() - 1; i >= 0; --i) 
	{
		if (m_pEnemyCurrentWeaponAttacks->pos[i].entityId == enemyID)
		{
			m_pEnemyCurrentWeaponAttacks->pos.erase(m_pEnemyCurrentWeaponAttacks->pos.begin() + i);
		}
	}

	m_pEnemy->entity[index].Clean();

	m_pEnemy->pos.erase(m_pEnemy->pos.begin() + index);
	m_pEnemy->entity.erase(m_pEnemy->entity.begin() + index);

	return	true;
}

void	CEntityManager::ProcessEnemyAttack(std::vector<int> &enemyIndex, int playerX, int playerY)
{
	for (size_t i = 0; i < enemyIndex.size(); ++i)
	{
		CEntity *Entity = &m_pEnemy->entity[enemyIndex[i]];

		// for now just take the first attack available
		WeaponAttackType type{ Entity->GetWeaponAttack()->GetWeaponAttackDataC()->at(0).attackType };

		const	CWeapon *weapon{ Entity->GetEquipped()->GetWeaponC() };

		if (Entity->GetWeaponAttack()->IsAttackAvailable(type))
		{
			bool attackFailed = false;

			_mapPosWeaponAttack attack{};

			Entity->StartAttack(type, &attack);

			m_pAI->GetAIData().at(enemyIndex[i]).movementDisabled = true;

			attack.speed *= weapon->GetSpeed();

			if (type == WeaponAttackType::normalSlash)
			{
				attack.imgIndex = weapon->GetImgIndex();

				int entityX = static_cast<int>(m_pEnemy->pos[enemyIndex[i]].x), entityY = static_cast<int>(m_pEnemy->pos[enemyIndex[i]].y);
				int	tEntityX = entityX + 16, tEntityY = entityY + 16;

				if (playerX <= tEntityX && playerX >= tEntityX - 60)
				{
					if (playerX >= tEntityX - 22 && playerY < tEntityY && playerY >= tEntityY - 60) // top
					{
						attack.posX = static_cast<float>(entityX), attack.posY = entityY - 32.f, attack.currentValue = 0;
					}
					else if (playerX >= tEntityX - 22 && playerY >= tEntityY && playerY <= tEntityY + 60) // down
					{
						attack.posX = static_cast<float>(entityX), attack.posY = entityY + 32.f, attack.currentValue = 180;
					}
					else if (playerY >= tEntityY - 60 && playerY < tEntityY - 32) // top left
					{
						attack.posX = entityX - 32.f, attack.posY = entityY - 32.f, attack.currentValue = 0;
					}
					else if (playerY >= tEntityY - 32 && playerY < tEntityY + 8) // left
					{
						attack.posX = entityX - 32.f, attack.posY = static_cast<float>(entityY), attack.currentValue = -90;
					}
					else if (playerY >= tEntityY + 8 && playerY < tEntityY + 60) // down left
					{
						attack.posX = entityX - 32.f, attack.posY = entityY + 32.f, attack.currentValue = -90;
					}
					else
					{
						attackFailed = true;
					}
				}
				else if (playerX > tEntityX && playerX <= tEntityX + 60)
				{
					if (playerX <= tEntityX + 6 && playerY < tEntityY && playerY >= tEntityY - 60) // top
					{
						attack.posX = static_cast<float>(entityX), attack.posY = entityY - 32.f, attack.currentValue = 0;
					}
					else if (playerX <= tEntityX + 6 && playerY >= tEntityY && playerY <= tEntityY + 60) // down
					{
						attack.posX = static_cast<float>(entityX), attack.posY = entityY + 32.f, attack.currentValue = 180;
					}
					else if (playerY >= tEntityY - 60 && playerY < tEntityY - 32) // top right
					{
						attack.posX = entityX + 32.f, attack.posY = entityY - 32.f, attack.currentValue = 0;
					}
					else if (playerY >= tEntityY - 32 && playerY < tEntityY + 8) // right
					{
						attack.posX = entityX + 32.f, attack.posY = static_cast<float>(entityY), attack.currentValue = 90;
					}
					else if (playerY >= tEntityY + 8 && playerY < tEntityY + 60) // down right
					{
						attack.posX = entityX + 32.f, attack.posY = entityY + 32.f, attack.currentValue = 0;
					}
					else
					{
						attackFailed = true;
					}
				}
				else
				{
					attackFailed = true;
				}

				attack.value += attack.currentValue;

				if (attack.posX < 0)
					attack.posX = 0;

				if (attack.posY < 0)
					attack.posY = 0;
			}

			if (!attackFailed)
			{
				m_pEnemyCurrentWeaponAttacks->pos.emplace_back(attack);
			}
			else
			{
				Entity->GetWeaponAttack()->SetAttackStarted(false);
				m_pAI->GetAIData().at(enemyIndex[i]).movementDisabled = false;
			}
		}
	}

	enemyIndex.clear();
}

void	CEntityManager::ProcessPlayerAttack(const KeyboardEvents &key, const _leftRight &lR, const _upDown &uD, float posX, float posY)
{
	if (uD.state == InputState::nothing && lR.state == InputState::nothing)
		return;

	CEntity *Entity = m_pPlayer->GetEntity();

	WeaponAttackType	type{ Entity->GetWeaponAttack()->GetMappedAttack(key) };

	const	CWeapon	*weapon{ Entity->GetEquipped()->GetWeaponC() };

	// check if the attack is off cooldown and if the attack is available for the weapon
	if (m_pItemMng->IsAttackLegal(weapon, type) && Entity->GetWeaponAttack()->IsAttackAvailable(type))
	{
		_mapPosWeaponAttack attack{};

		Entity->StartAttack(type, &attack);

		attack.speed *= weapon->GetSpeed();

		if (type == WeaponAttackType::normalSlash)
		{
			attack.imgIndex = weapon->GetImgIndex();

			// currentValue is the current angle of the image

			if (uD.state != InputState::nothing)
			{
				if (uD.upDown == UpDown::Down)
				{
					if (lR.state != InputState::nothing && lR.leftRight == LeftRight::Left)
					{
						attack.posX = posX - 32, attack.posY = posY + 32, attack.currentValue = -90;
					}
					else if (lR.state != InputState::nothing && lR.leftRight == LeftRight::Right)
					{
						attack.posX = posX + 32, attack.posY = posY + 32, attack.currentValue = 135;
					}
					else
					{
						attack.posX = posX, attack.posY = posY + 32, attack.currentValue = 180, attack.direction = WeaponAttackDirection::down;
					}
				}
				else if (uD.upDown == UpDown::Up)
				{
					if (lR.state != InputState::nothing && lR.leftRight == LeftRight::Left)
					{
						attack.posX = posX - 32, attack.posY = posY - 32, attack.currentValue = 0;
					}
					else if (lR.state != InputState::nothing && lR.leftRight == LeftRight::Right)
					{
						attack.posX = posX + 32, attack.posY = posY - 32, attack.currentValue = 0;
					}
					else
					{
						attack.posX = posX, attack.posY = posY - 32, attack.currentValue = 0, attack.direction = WeaponAttackDirection::up;
					}
				}
			}
			else if (lR.state != InputState::nothing)
			{
				if (lR.leftRight == LeftRight::Left)
					attack.posX = posX - 32, attack.posY = posY, attack.currentValue = -90, attack.direction = WeaponAttackDirection::left;
				else if (lR.leftRight == LeftRight::Right)
					attack.posX = posX + 32, attack.posY = posY, attack.currentValue = 90, attack.direction = WeaponAttackDirection::right;
			}

			attack.value += attack.currentValue; // update the finish angle 
		}

		m_pPlayerCurrentWeaponAttacks->pos.push_back(attack);
	}
}

void	CEntityManager::PlayerAttackHit(const int attackIndex, const int enemyID)
{
	int enemyIndex{};

	CEntity *Enemy{ GetEnemyById(enemyID, enemyIndex) };

	if (Enemy == nullptr)
		return;

	const CWeapon *Weapon{ m_pPlayer->GetEntity()->GetEquipped()->GetWeaponC() };

	int attack = Weapon->GetAttack();

	attack = static_cast<int>(attack * m_pPlayer->GetEntity()->GetWeaponAttack()->GetAttackModifier(m_pPlayerCurrentWeaponAttacks->pos[attackIndex].attackType));

	Enemy->DecreaseHp(attack);

	if (!Enemy->IsAlive())
	{
		EnemyDied(enemyIndex);
	}
}

void	CEntityManager::EnemyAttackHit(int attackIndex)
{
	if (attackIndex < 0 || attackIndex >= static_cast<int>(m_pEnemyCurrentWeaponAttacks->pos.size()))
		return;

	int enemyID{ m_pEnemyCurrentWeaponAttacks->pos[attackIndex].entityId };

	CEntity *Enemy{ GetEnemyById(enemyID) };

	if (Enemy == nullptr)
		return;

	const CWeapon *Weapon{ Enemy->GetEquipped()->GetWeaponC() };

	int weapAttack{ Weapon->GetAttack() };

	int attack{ static_cast<int>(weapAttack * Enemy->GetWeaponAttack()->GetAttackModifier(m_pEnemyCurrentWeaponAttacks->pos[attackIndex].attackType)) };

	m_pPlayer->GetEntity()->DecreaseHp(attack);
}

bool	CEntityManager::EnemyDied(const int index)
{
	if (!DeleteEnemy(index))
		return	false;

	return	true;
}

void		CEntityManager::UpdateInGameHoverData(int mouseX1, int mouseY1, int cameraX, int cameraY, const CSpellManager *SpellMng)
{
	if (m_inGameHoverDataTime > 0)
		return;

	m_inGameHoverDataTime = m_inGameHoverDataTimeReset;

	const int currentIndex = m_pInGameHoverData->inGameHoverIndex;
	InGameHoverType currentType = m_pInGameHoverData->inGameHoverType;

	int		entityX{}, entityY{};

	InGameHoverType	skip{ InGameHoverType::nothing };

	if (currentType == InGameHoverType::enemy)
	{
		entityX = static_cast<int>(m_pEnemy->pos[currentIndex].x), entityY = static_cast<int>(m_pEnemy->pos[currentIndex].y);

		if (UpdateHoverOverEnemy(mouseX1, mouseY1, cameraX, cameraY, entityX, entityY, currentIndex))
			return;
	}
	else if (currentType == InGameHoverType::hudSpell)
	{
		if (UpdateHoverOverHudSpell(SpellMng, mouseX1, mouseY1))
			return;

		skip = InGameHoverType::hudSpell;
	}
	else if (currentType == InGameHoverType::hudWeapAtt)
	{
		if (UpdateHoverOverHudWeapAtt(mouseX1, mouseY1))
			return;

		skip = InGameHoverType::hudWeapAtt;
	}

	if (skip != InGameHoverType::hudSpell)
	{
		if (UpdateHoverOverHudSpell(SpellMng, mouseX1, mouseY1))
			return;
	}

	if (skip != InGameHoverType::hudWeapAtt)
	{
		if (UpdateHoverOverHudWeapAtt(mouseX1, mouseY1))
			return;
	}

	for (size_t i = 0; i < m_pEnemy->pos.size(); i++)
	{
		entityX = static_cast<int>(m_pEnemy->pos[i].x), entityY = static_cast<int>(m_pEnemy->pos[i].y);

		if (UpdateHoverOverEnemy(mouseX1, mouseY1, cameraX, cameraY, entityX, entityY, static_cast<int>(i)))
			return;
	}

	EmptyHoverData();
}

bool	CEntityManager::UpdateHoverOverHudWeapAtt(int mouseX1, int mouseY1)
{
	const	_inGameHudData *hudData{ m_pItemMng->GetPlayerHUDWeapAtt() };

	int	x{ hudData->x }, y{ hudData->y + g_windowHeight + 32 }; // hudData->y is on the hud texture for weap attacks, 32 is for the position on the hud texture for everything
																// and + g_windowHeight for the window position

	if (!DD::CheckMouseCollision(mouseX1, mouseY1, x, y, hudData->offsetX * hudData->index.size(), hudData->h))
		return	false;

	for (size_t i = 0; i < hudData->index.size(); ++i)
	{
		if (DD::CheckMouseCollision(mouseX1, mouseY1, x, y, hudData->w, hudData->h))
		{
			m_pInGameHoverData->data.clear();

			const _weaponAttackData &weapAtt{ *m_pItemMng->GetWeapAttC(hudData->index[i]) };

			m_pInGameHoverData->data.emplace_back(weapAtt.name);
			m_pInGameHoverData->data.emplace_back("Damage: " + DD::GetPreciseStr(weapAtt.attackModifier * 100, 0) + "%");
			m_pInGameHoverData->data.emplace_back("Speed: " + DD::GetPreciseStr(weapAtt.speed, 2));
			m_pInGameHoverData->data.emplace_back("Cooldown: " + DD::GetPreciseStr(weapAtt.cooldownReset, 2));

			m_pInGameHoverData->w = 128;
			m_pInGameHoverData->h = 62;
			m_pInGameHoverData->posX = (x + hudData->w / 2) - m_pInGameHoverData->w / 2; //x;
			m_pInGameHoverData->posY = g_windowHeight - m_pInGameHoverData->h - 2;

			m_pInGameHoverData->inGameHoverType = InGameHoverType::hudWeapAtt;

			return	true;
		}

		x += hudData->offsetX;
	}

	return	false;
}

bool	CEntityManager::UpdateHoverOverHudSpell(const CSpellManager *SpellMng, int mouseX1, int mouseY1)
{
	const _inGameHudData *HUDSpells{ SpellMng->GetPlayerHudSpells() };

	int x{ HUDSpells->x }, y{ HUDSpells->y + g_windowHeight };

	if (!DD::CheckMouseCollision(mouseX1, mouseY1, x, y, HUDSpells->offsetX * HUDSpells->index.size(), HUDSpells->h)) // mouse over any hud spell ?
		return	false;

	for (size_t i = 0; i < HUDSpells->index.size(); ++i)
	{
		if (DD::CheckMouseCollision(mouseX1, mouseY1, x, y, HUDSpells->w, HUDSpells->h))
		{
			m_pInGameHoverData->data.clear();

			const CSpell *Spell = SpellMng->GetSpellC(HUDSpells->index[i]);

			SpellList spell_ = Spell->GetSpell();

			std::string nameStr{ SpellMng->GetSpellName(spell_) };

			m_pInGameHoverData->data.push_back(nameStr);

			SpellType spellType = Spell->GetSpellType();

			if (spellType == SpellType::damage)
			{
				std::string dmgStr{ "Damage:" + std::to_string(Spell->GetValue()) };
				m_pInGameHoverData->data.push_back(dmgStr);
			}

			std::string costStr{ "MP cost:" + std::to_string(Spell->GetManaCost()) };
			std::string cdStr{ "Cooldown:" + DD::GetPreciseStr(Spell->GetCooldown(), 2) };
			std::string speedStr{ "Speed:" + std::to_string(Spell->GetSpeed()) };

			m_pInGameHoverData->data.push_back(costStr);
			m_pInGameHoverData->data.push_back(cdStr);
			m_pInGameHoverData->data.push_back(speedStr);

			m_pInGameHoverData->w = 128;
			m_pInGameHoverData->h = 62;
			m_pInGameHoverData->posX = (x + HUDSpells->w / 2) - m_pInGameHoverData->w / 2; //x;
			m_pInGameHoverData->posY = g_windowHeight - m_pInGameHoverData->h - 2;
			m_pInGameHoverData->inGameHoverIndex = HUDSpells->index[i]; // not needed
			m_pInGameHoverData->inGameHoverType = InGameHoverType::hudSpell;

			return	true;
		}

		x += HUDSpells->offsetX;
	}

	return	false;
}

bool	CEntityManager::UpdateHoverOverEnemy(int mouseX1, int mouseY1, int cameraX, int cameraY, int entityX, int entityY, int entityIndex)
{
	// get window positions
	entityX = entityX - cameraX + g_windowWidth / 2;
	entityY = entityY - cameraY + g_windowHeight / 2;

	if (DD::CheckMouseCollision(mouseX1, mouseY1, entityX, entityY, 32, 32) && (entityY + 32 < g_windowHeight)) // is the mouse over the entity and is the entity not touching windowHUD
	{
		if (entityX < g_windowWidth - 101) // is there enough space for the tooltip to be shown right of the entity
		{
			m_pInGameHoverData->posX = entityX + 32 + 5;
			m_pInGameHoverData->posY = entityY;
		}
		else // show it to the left of the entity
		{
			m_pInGameHoverData->posX = entityX - (64 + 5);
			m_pInGameHoverData->posY = entityY;
		}

		m_pInGameHoverData->w = 76;
		m_pInGameHoverData->h = 40;

		m_pInGameHoverData->data.clear();

		int hp = m_pEnemy->entity[entityIndex].GetHp();
		int mp = m_pEnemy->entity[entityIndex].GetMp();

		std::string hpStr{ "HP:" + std::to_string(hp) };
		std::string mpStr{ "MP:" + std::to_string(mp) };
		std::string name{ m_enemyNameMap[m_pEnemy->entity[entityIndex].GetEnemyType()] };

		size_t found = name.find(" ");

		if (found != std::string::npos && name.length() > 10) // check so that if two words can't be in the same row they will get split 
		{
			m_pInGameHoverData->data.push_back(name.substr(0, found));
			m_pInGameHoverData->data.push_back(name.substr(found + 1));
		}
		else
			m_pInGameHoverData->data.push_back(name);

		m_pInGameHoverData->data.push_back(hpStr);
		m_pInGameHoverData->data.push_back(mpStr);
		m_pInGameHoverData->inGameHoverType = InGameHoverType::enemy;
		m_pInGameHoverData->inGameHoverIndex = entityIndex;


		return	true;
	}

	return	false;
}

void	CEntityManager::EmptyHoverData()
{
	m_pInGameHoverData->data.clear();
	m_pInGameHoverData->posX = 0;
	m_pInGameHoverData->posY = 0;
	m_pInGameHoverData->w = 0;
	m_pInGameHoverData->h = 0;
	m_pInGameHoverData->inGameHoverIndex = -1;
	m_pInGameHoverData->inGameHoverType = InGameHoverType::nothing;
}

void	CEntityManager::DeleteCurrentPlayerWeaponAttack(int	index, bool attackHasExpired)
{
	if (index < 0 || index >= static_cast<int>(m_pPlayerCurrentWeaponAttacks->pos.size()))
		return;

	// if the attack hasn't expired we do not delete it if deleteAfterCollision is false
	if (!m_pPlayerCurrentWeaponAttacks->pos[index].deleteAfterCollision && !attackHasExpired)
		return;

	m_pPlayer->GetEntity()->GetWeaponAttack()->SetAttackStarted(false);

	m_pPlayerCurrentWeaponAttacks->pos.erase(m_pPlayerCurrentWeaponAttacks->pos.begin() + index);
}

void	CEntityManager::DeleteCurrentEnemyWeaponAttack(int attackIndex, bool attackHasExpired)
{
	if (attackIndex < 0 || attackIndex >= static_cast<int>(m_pEnemyCurrentWeaponAttacks->pos.size()))
		return;

	int enemyId{ m_pEnemyCurrentWeaponAttacks->pos[attackIndex].entityId };

	int enemyIndex{};

	CEntity *Enemy{ GetEnemyById(enemyId, enemyIndex) };

	if (Enemy == nullptr) // must check if an enemy is still alive because if it's dead it's attack needs to be deleted
	{
		for (int i = m_pEnemyCurrentWeaponAttacks->pos.size() - 1; i >= 0; --i) // enemyIndex is -1 so we can not use is for attack deletion
		{
			if (m_pEnemyCurrentWeaponAttacks->pos[i].entityId == enemyId)
			{
				m_pEnemyCurrentWeaponAttacks->pos.erase(m_pEnemyCurrentWeaponAttacks->pos.begin() + i);
			}
		}

		return;
	}

	if (!m_pEnemyCurrentWeaponAttacks->pos[attackIndex].deleteAfterCollision && !attackHasExpired)
	{
		return;
	}

	if (Enemy != nullptr) // Enemy still alive
	{
		m_pEnemy->entity[enemyIndex].GetWeaponAttack()->SetAttackStarted(false);
		
		m_pAI->GetAIData().at(enemyIndex).movementDisabled = false;
	}

	m_pEnemyCurrentWeaponAttacks->pos.erase(m_pEnemyCurrentWeaponAttacks->pos.begin() + attackIndex);
}

CItemManager*	CEntityManager::GetItemManager()
{
	return	m_pItemMng;
}

CPlayer*	CEntityManager::GetPlayer()
{
	return	m_pPlayer;
}

CAI*		CEntityManager::GetAI()
{
	return	m_pAI;
}

_weaponAttackPosWrapper*	CEntityManager::GetCurrentWeaponAttacksForPlayer()
{
	return	m_pPlayerCurrentWeaponAttacks;
}

_weaponAttackPosWrapper*	CEntityManager::GetCurrentWeaponAttacksForEnemy()
{
	return	m_pEnemyCurrentWeaponAttacks;
}

CEntity*	CEntityManager::GetEnemyById(const int id)
{
	for (auto& enemy : m_pEnemy->entity)
	{
		if (enemy.GetId() == id)
			return	&enemy;
	}

	return	nullptr;
}

CEntity*	CEntityManager::GetEnemyById(int id, int &index)
{
	std::vector<CEntity>& enemy{ m_pEnemy->entity };

	for (size_t i = 0; i < enemy.size(); ++i)
	{
		if (enemy[i].GetId() == id)
		{
			index = static_cast<int>(i);

			return	&enemy[i];
		}
	}

	index = -1;

	return	nullptr;
}

const _inGameHoverData*	CEntityManager::GetInGameHoverData() const
{
	return	m_pInGameHoverData;
}
