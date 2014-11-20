#include "CEntity.h"
#include "Item/CEquipped.h"
#include "Item/CWeaponAttack.h"
#include "EntityData.h"
#include "../../Data/Globals.h"
#include "../../Tools/debug.h"

CEntity::CEntity()
{
	m_hp = 0;
	m_mp = 0;
	m_entityType = EntityType::nothing;
	m_enemyType = EnemyType::nothing;
	m_id = CGlobal::GetEntityID();
	m_pEquipped = nullptr;
	m_pWeaponAttacks = nullptr;
}

CEntity::CEntity(int hp, int mp, int speed, EnemyType type)
{
	m_hp = hp;
	m_mp = mp;
	m_speed = speed;
	m_entityType = EntityType::monster;
	m_enemyType = type;
	m_id = CGlobal::GetEntityID();
	m_pEquipped = nullptr;
	m_pWeaponAttacks = nullptr;

	InitEquipped();
	InitWeaponAttacks();
}

CEntity::~CEntity()
{

}

bool	CEntity::Init(int hp, int mp, int speed)
{
	m_hp = hp;
	m_mp = mp;
	m_speed = speed;

	return	true;
}

void	CEntity::Clean()
{
	DD_DELETE(m_pEquipped);
	DD_DELETE(m_pWeaponAttacks);
}

void	CEntity::InitEquipped()
{
	m_pEquipped = DD_NEW CEquipped{};
}

void	CEntity::InitWeaponAttacks()
{
	m_pWeaponAttacks = DD_NEW CWeaponAttack{};
}

void	CEntity::StartAttack(WeaponAttackType type, _mapPosWeaponAttack *attackPos)
{
	attackPos->attackType = type;
	attackPos->enemyType = m_enemyType;
	attackPos->entityType = m_entityType;
	attackPos->entityId = m_id;
	attackPos->speed = m_pWeaponAttacks->GetSpeed(type);
	attackPos->value = m_pWeaponAttacks->GetValue(type);
	attackPos->currentValue = 0;
	attackPos->deleteAfterCollision = false;

	m_pWeaponAttacks->SetAttackStarted(true);
	m_pWeaponAttacks->StartAttackTimer(type);
}

void	CEntity::Update(double deltaTime)
{
	m_pWeaponAttacks->Update(deltaTime);
}

bool	CEntity::IsAlive() const
{
	if (m_hp > 0)
		return	true;
	return	false;
}

int		CEntity::GetHp() const
{
	return	m_hp;
}

void	CEntity::SetHp(const int value)
{
	m_hp = value;
}

void	CEntity::IncreaseHp(const int value)
{
	m_hp += value;
}

void	CEntity::DecreaseHp(const int value)
{
	m_hp -= value;
}

int		CEntity::GetMp() const
{
	return	m_mp;
}

void	CEntity::SetMp(const int value)
{
	m_mp = value;
}

void	CEntity::IncreaseMp(const int value)
{
	m_mp += value;
}

void	CEntity::DecreaseMp(const int value)
{
	m_mp -= value;
}

void	CEntity::SetEntityType(const EntityType &type)
{
	m_entityType = type;
}

EntityType CEntity::GetEntityType() const
{
	return	m_entityType;
}

EnemyType CEntity::GetEnemyType() const
{
	return	m_enemyType;
}

int	CEntity::GetSpeed() const
{
	return	m_speed;
}

void	CEntity::SetSpeed(int value)
{
	m_speed = value;
}

CEquipped*	CEntity::GetEquipped()
{
	return	m_pEquipped;
}

CWeaponAttack*	CEntity::GetWeaponAttack()
{
	return	m_pWeaponAttacks;
}

int	CEntity::GetId() const
{
	return	m_id;
}
