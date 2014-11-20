#pragma once

class CEntity;

class CPlayer
{
	CEntity		*m_pEntity;

public:

	CPlayer();
	~CPlayer();

	bool	Init(int hp, int mp, int speed);
	void	Clean();

	CEntity*	GetEntity();
	const CEntity* GetEntityC() const;
};
