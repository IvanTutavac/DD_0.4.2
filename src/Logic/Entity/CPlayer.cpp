#include "CPlayer.h"
#include "CEntity.h"
#include "EntityData.h"
#include "../../Tools/debug.h"
#include "../../Tools/Log.h"

CPlayer::CPlayer()
{
	m_pEntity = nullptr;
}

CPlayer::~CPlayer()
{

}

bool	CPlayer::Init(int hp, int mp, int speed)
{
	m_pEntity = DD_NEW CEntity{};

	if (!m_pEntity->Init(hp, mp, speed))
	{
		Log("CPlayer::Init() failed, Entity::Init() caused fail");
		return	false;
	}

	m_pEntity->SetEntityType(EntityType::player);

	m_pEntity->InitEquipped();

	m_pEntity->InitWeaponAttacks();

	return	true;
}

void	CPlayer::Clean()
{
	if (m_pEntity != nullptr)
		m_pEntity->Clean();

	DD_DELETE(m_pEntity);
}

CEntity*	CPlayer::GetEntity()
{
	return	m_pEntity;
}

const CEntity*	CPlayer::GetEntityC() const
{
	return	m_pEntity;
}
