#pragma once

enum class EntityType;
enum class EnemyType;
enum class WeaponAttackType;

struct _mapPosWeaponAttack;

class CEquipped;
class CWeaponAttack;

class CEntity
{
private:

	CEquipped *m_pEquipped;
	CWeaponAttack *m_pWeaponAttacks;

	int		m_hp;
	int		m_mp;
	int		m_speed;

	int		m_id; 

	EntityType	m_entityType;
	EnemyType	m_enemyType;

public:

	CEntity();
	CEntity(int hp, int mp, int speed, EnemyType type);
	~CEntity();

	bool	Init(int hp, int mp, int speed);
	void	Clean();

	void	InitEquipped();
	void	InitWeaponAttacks();

	void	StartAttack(WeaponAttackType type, _mapPosWeaponAttack *attackPos);

	void	Update(double deltaTime);

	bool	IsAlive() const;
	int		GetHp() const;
	void	SetHp(const int value);
	void	IncreaseHp(const int value);
	void	DecreaseHp(const int value);

	int		GetMp() const;
	void	SetMp(const int value);
	void	IncreaseMp(const int value);
	void	DecreaseMp(const int value);

	int		GetSpeed() const;
	void	SetSpeed(int value);

	int		GetId() const;

	void	SetEntityType(const EntityType &type);
	EntityType GetEntityType() const;
	EnemyType GetEnemyType() const;

	CWeaponAttack*	GetWeaponAttack();
	CEquipped* GetEquipped();
};
