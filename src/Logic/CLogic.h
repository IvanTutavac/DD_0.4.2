#pragma once

#include "../Data/Const.h"

#include <vector>

class CPlayer;
class CEntity;
class CEntityManager;
class CMovement;
class CCollision;
class CMapEditor;
class CMap;
class CMapInGame;
class CState;
class CGUI;
class CSpellManager;
class CMessage;
class CMessageToLogic;

enum class RenderAPI;
enum class KeyboardEvents;
class CGUIAction;

struct _keyboardEvents;
struct _leftRight;
struct _upDown;

class CLogic
{
private:

	CEntityManager	*m_pEntityManager;
	CMapEditor		*m_pMapEditor;
	CMapInGame		*m_pMap;
	CMovement		*m_pMovement;
	CCollision		*m_pCollision;
	CState			*m_pState;
	CGUI			*m_pGUI;
	CSpellManager	*m_pSpellManager;

	RenderAPI m_renderAPI;

	double	m_deltaTime; // Set each frame by UpdateLogic

	double  m_textInputTime; // if its <= 0 process the text input
	const double m_textInputTimeReset = 0.12;

	float	m_previousPlayerX;
	float	m_previousPlayerY;

	bool	m_waitingForAttackDirection; // button for attacking is being pressed, direction key press will result into an attack, not movement

	bool	m_playerMovementByMouse;

	bool	m_floorPassable[IMG_FLOOR_FINISH + 1];

private:

	void	UpdateTime();

	bool	AI();

	void	Movement(CMessage *message);

	void	UpdateCamera(int mouseX, int mouseY, CMap *Map);

	bool	Collision();
	bool	SpellCollision(std::vector<std::pair<int, int>> &collided);
	void	WeaponCollisionOnEnemy(std::vector<std::pair<int, int>> &collided);
	void	WeaponCollisionOnPlayer(std::vector<std::pair<int, int>> &collided);

	bool	ProcessGUIInput(CMessage *message);

	bool	GetGUIAction(CMessage *message, CGUIAction *guiAction);
	bool	ReadGUIAction(CMessage *message, CGUIAction *guiAction);

	bool	HandleTextInput(CMessage *message);

	void	ReadMsgToLogic(CMessageToLogic *message);

	bool	ProcessPlayerInput(CMessage *message);
	bool	CastPlayerSpell(const KeyboardEvents &key, float mouseX, float mouseY);

	void	MapMouseClickToKeys(const CMessage *Msg, _leftRight &lR, _upDown &uD);
	void	MapKeyboardMovementToMouse(float &mouseX, float &mouseY, const _leftRight &lR, const _upDown &uD, float playerX, float playerY);

public:

	CLogic();
	~CLogic();

	bool	Init(const RenderAPI &api);
	void	Clean();

public:

	// main logic function
	bool	UpdateLogic(const double deltaTime,CMessage *message);

	CState*	GetState();
	CMapEditor* GetMapEditor();
	CMapInGame* GetMap();
	CGUI* GetGUI();
	CSpellManager *GetSpellManager();
	CEntityManager* GetEntityManager();
	CPlayer*	GetPlayer();
	const CEntity*	GetPlayerEntityC() const;
	CCollision*	GetCollision();
};
