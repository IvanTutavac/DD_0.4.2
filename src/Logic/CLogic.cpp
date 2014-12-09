#include "CLogic.h"
#include "Map/CMapEditor.h"
#include "Map/CMapInGame.h"
#include "Physics/CMovement.h"
#include "Physics/CCollision.h"
#include "State/CState.h"
#include "Spell/CSpellManager.h"
#include "Spell/SpellData.h"
#include "Spell/CSpell.h"
#include "Entity/EntityData.h"
#include "Entity/CEntity.h"
#include "Entity/CPlayer.h"
#include "Entity/CEntityManager.h"
#include "Entity/CAI.h"
#include "Entity/Item/CWeaponAttack.h"
#include "GUI/CGUI.h"
#include "GUI/CWindow.h"
#include "GUI/CTextBox.h"
#include "GUI/CSelectBox.h"
#include "GUI/CGUIAction.h"
#include "GUI/EnumGui.h"
#include "../IO/IOFunctions.h"
#include "../Tools/MessageBox.h"
#include "../Tools/debug.h"
#include "../Tools/Log.h"
#include "../Message/CMessage.h"
#include "../Data/Globals.h"
#include "../Data/Enums.h"
#include "../Input/InputData.h"

CLogic::CLogic()
{
	m_pEntityManager = nullptr;
	m_pMap = nullptr;
	m_pMapEditor = nullptr;
	m_pMovement = nullptr;
	m_pState = nullptr;
	m_pGUI = nullptr;
	m_pSpellManager = nullptr;
	m_pCollision = nullptr;

	m_deltaTime = 0;
}

CLogic::~CLogic()
{

}

bool	CLogic::Init(const RenderAPI &api)
{
	m_pEntityManager = DD_NEW CEntityManager{};
	m_pMap = DD_NEW CMapInGame{};
	m_pMapEditor = DD_NEW CMapEditor{};
	m_pMovement = DD_NEW CMovement{};
	m_pState = DD_NEW CState{};
	m_pGUI = DD_NEW CGUI{};
	m_pSpellManager = DD_NEW CSpellManager{};
	m_pCollision = DD_NEW CCollision{};

	if (m_pMap->Init() == false)
	{
		Log("CLogic::Init failed, Map Init failed");
		return	false;
	}

	if (m_pMapEditor->Init() == false)
	{
		Log("CLogic::Init failed, MapEditor Init failed");
		return	false;
	}

	if (!DD::IO::LoadPassableFloor(m_floorPassable))
	{
		Log("CLogic::Init() failed, DD::IO::LoadPassableFloor caused the fail");
		return	false;
	}

	if (m_pEntityManager->Init(m_pMap->GetCurrentMap(), m_floorPassable) == false)
	{
		Log("Clogic::Init failed, Entity Manager Init failed");
		return	false;
	}

	if (m_pMovement->Init() == false)
	{
		Log("CLogic::Init failed, Movement Init  failed");
		return	false;
	}

	if (m_pState->Init() == false)
	{
		Log("CLogic::Init failed, State Init failed");
		return	false;
	}

	if (m_pGUI->Init() == false)
	{
		Log("CLogic::Init failed, GUI Init failed");
		return	false;
	}

	if (m_pSpellManager->Init() == false)
	{
		Log("CLogic::Init failed, SpellManager Init failed");
		return	false;
	}

	if (!m_pCollision->Init())
	{
		Log("CLogic::Init failed, Collision init failed");
		return	false;
	}

	m_pState->SetGameStateMainMenu();
	//m_pState->SetGameStateMapEditor();
	//m_pMapEditor->SetInAllTiles();
	m_pMapEditor->SetMapSmall();

	m_renderAPI = api;

	m_waitingForAttackDirection = false;
	m_playerMovementByMouse = false;

	m_textInputTime = m_textInputTimeReset;

	return	true;
}

void	CLogic::Clean()
{
	if (m_pEntityManager != nullptr)
		m_pEntityManager->Clean();
	if (m_pMap != nullptr)
		m_pMap->Clean();
	if (m_pMapEditor != nullptr)
		m_pMapEditor->Clean();
	if (m_pMovement != nullptr)
		m_pMovement->Clean();
	if (m_pState != nullptr)
		m_pState->Clean();
	if (m_pGUI != nullptr)
		m_pGUI->Clean(); 
	if (m_pSpellManager != nullptr)
		m_pSpellManager->Clean();
	if (m_pCollision != nullptr)
		m_pCollision->Clean();

	DD_DELETE(m_pEntityManager);
	DD_DELETE(m_pMap);
	DD_DELETE(m_pMapEditor);
	DD_DELETE(m_pMovement);
	DD_DELETE(m_pState);
	DD_DELETE(m_pGUI);
	DD_DELETE(m_pSpellManager);
	DD_DELETE(m_pCollision);
}

bool	CLogic::UpdateLogic(const double deltaTime, CMessage *message)
{
	m_deltaTime = deltaTime;

	UpdateTime();

	AI();

	Movement(message);

	if (!Collision())
		return	false;

	if (!ProcessPlayerInput(message))
		return	false;

	if (!ProcessGUIInput(message))
		return	false;

	ReadMsgToLogic(message->GetMsgToLogic());

	message->ClearInputMsg(false);

	return	true;
}

void	CLogic::UpdateTime()
{
	m_pGUI->Update(m_deltaTime);
	m_pSpellManager->Update(m_deltaTime);
	m_pEntityManager->Update(m_deltaTime);

	if (m_textInputTime > 0)
		m_textInputTime -= m_deltaTime;
}

bool	CLogic::AI()
{
	if (!m_pState->IsGameStateInGame())
		return	true;

	if (!m_pEntityManager->UpdateAI(static_cast<int>(m_pMap->GetPlayerX()), static_cast<int>(m_pMap->GetPlayerY())))
	{
		Log("CLogic::AI() failed, CEntityManager::UpdateAI(int,int) caused the fail");
		return	false;
	}

	return	true;
}

bool	CLogic::ProcessPlayerInput(CMessage *message)
{
	const _inputMessage *InputMsg = message->GetInputMsg();

	const _keyboardEvents key = InputMsg->key;
	const _upDown upDown = InputMsg->upDown;
	const _leftRight leftRight = InputMsg->leftRight;

	const float mouseX1{ static_cast<float>(InputMsg->x1) };
	const float mouseY1{ static_cast<float>(InputMsg->y1) };

	if (m_pState->IsGameStateInGame()) // fali check da nismo u izbornicima, ako jesmo  gledalo bi se drugacije
	{
		if (key.state == InputState::released)
		{
			switch (key.key)
			{
			case	KeyboardEvents::one:
			case	KeyboardEvents::two:
				CastPlayerSpell(key.key, mouseX1, mouseY1);
				break;
			case	KeyboardEvents::W:
			case	KeyboardEvents::Q:
				m_waitingForAttackDirection = false;
			default:
				break;
			}
		}
		else if (key.state == InputState::pressed)
		{
			switch (key.key)
			{
			case	KeyboardEvents::W:
			case	KeyboardEvents::Q:
			{
				_leftRight lR = leftRight;
				_upDown uD = upDown;

				MapMouseClickToKeys(message, lR, uD);
				m_pEntityManager->ProcessPlayerAttack(key.key, lR, uD, m_pMap->GetPlayerX(), m_pMap->GetPlayerY());
				m_waitingForAttackDirection = true;
				m_playerMovementByMouse = false;
				break;
			}
			default:
				break;
			}
		}

		m_pEntityManager->UpdateInGameHoverData(static_cast<int>(mouseX1), static_cast<int>(mouseY1), static_cast<int>(m_pMap->GetCameraX()), static_cast<int>(m_pMap->GetCameraY()), m_pSpellManager);
	}

	return	true;
}

bool	CLogic::CastPlayerSpell(const KeyboardEvents &key, float mouseX, float mouseY)
{
	SpellList	spell = m_pSpellManager->GetPlayerSpell(key);

	if (spell == SpellList::nothing)
		return	false;

	_spellCaster	caster{};

	caster.type = EntityType::player;

	int mp = m_pEntityManager->GetPlayer()->GetEntity()->GetMp();

	if (!m_pSpellManager->CanCast(caster, spell, mp))
		return	false;

	_mapPos	position{};

	// spells do not need an id... so the id will be used for chosing the caster, 0 player, 1 enemy
	position.id = 0;
	position.index = m_pSpellManager->GetSpellIndex(spell);
	position.x = m_pMap->GetPlayerX();
	position.y = m_pMap->GetPlayerY();
	position.speed = m_pSpellManager->GetSpellC(spell)->GetSpeed();

	m_pMovement->UpdateMapMovePosition(position, mouseX, mouseY,m_pMap->GetCameraX(),m_pMap->GetCameraY());

	m_pSpellManager->CastSpell(spell, caster, position , mp);

	m_pEntityManager->GetPlayer()->GetEntity()->SetMp(mp);

	return	true;
}

bool	CLogic::ProcessGUIInput(CMessage *message)
{
	CGUIAction	guiAction{};

	if (!HandleTextInput(message))
		return	false;

	if (!GetGUIAction(message, &guiAction))
		return	false;

	if (!ReadGUIAction(message, &guiAction))
		return	false;

	return	true;
}

bool	CLogic::HandleTextInput(CMessage *message)
{
	bool update = false;

	if (m_pGUI->GetCurrentWindow()->IsRecievingTextInput() == false)
		return	true;

	/* writes text to the only active text box */
	if (message->GetInputMsg()->written)
	{
		int	value = m_pGUI->GetCurrentWindow()->AddTextToTextBox(message->GetInputMsg()->letter);

		if (value == -1)
			return	false;

		//if (value == 0) // limit reached

		update = true;
	}

	if (message->GetInputMsg()->key.key == KeyboardEvents::Backspace && message->GetInputMsg()->key.state == InputState::pressed && m_textInputTime <= 0)
	{
		if (!m_pGUI->GetCurrentWindow()->DeleteLastLetterFromTextBox())
			return	false;

		update = true;
		m_textInputTime = m_textInputTimeReset;
	}

	if (update)
	{
		//message->SetUpdateTextBox();

		m_pGUI->GetCurrentWindow()->SetTextBoxDestroyWidgetData();

		message->SetDestroyWidgets();
		message->SetCreateTextBox();
	}

	return	true;
}

bool	CLogic::ReadGUIAction(CMessage *message, CGUIAction *guiAction)
{
	while (guiAction->IsWheel())
	{
		WidgetAction action{ guiAction->PopWheelAction() };

		if (action == WidgetAction::scrollSelectBox)
		{
			m_pGUI->GetCurrentWindow()->SetSelectBoxDestroyWidgetData();

			message->SetDestroyWidgets();
			message->SetCreateSelectBox();
		}
	}

	enum class ClickedWidget
	{
		nothing, clicked, clickedWidget
	};

	ClickedWidget	textInput{ ClickedWidget::nothing }; // used to check if a text box has been clicked
	ClickedWidget	selectBox{ ClickedWidget::nothing }; // used to check if a select box has been clicked

	//bool	textInput = m_pGUI->GetCurrentWindow()->IsRecievingTextInput();

	if (guiAction->IsPressRelease())
	{
		textInput = ClickedWidget::clicked;
		selectBox = ClickedWidget::clicked;

		//textInput = false;
	}

	while (guiAction->IsPressRelease())
	{
		WidgetAction actionPressRelease{ guiAction->PopPressReleaseAction() };

		if (actionPressRelease == WidgetAction::quitGame)
		{
			return	false;
		}
		else if (actionPressRelease == WidgetAction::enterNewGame)
		{
			m_pState->SetGameStateInGame();
			m_pGUI->ChangeCurrentWindow(GUI::WINDOW::WindowIndex::WindowIndex_inGame);
			m_pMap->SetMapShown();

			if (!m_pMap->LoadMap("test.DDmap"))
				return	false;

			_mapPos &playerMapPos{ m_pMap->GetPlayerMapPos() };

			playerMapPos.x = 64.f, playerMapPos.y = 64.f;
			playerMapPos.previousX = 64.f, playerMapPos.previousY = 64.f;
			playerMapPos.speed = m_pEntityManager->GetPlayer()->GetEntityC()->GetSpeed();

			m_pEntityManager->CreateEnemies(m_pMap->GetEnemyMapPos());

			m_pMap->DeleteEnemyMapPos();

			message->SetInitInGame();
			message->SetNewMapInGame();
			message->SetInGameHUDWeapAttData();
			message->SetInGameHUDSpellData();
			message->SetCloseAllTextBox();
			message->SetCloseAllSelectBox();
		}
		else if (actionPressRelease == WidgetAction::enterMainMenu)
		{
			m_pState->SetGameStateMainMenu();
			m_pGUI->ChangeCurrentWindow(GUI::WINDOW::WindowIndex::WindowIndex_mainMenu);
			message->SetInitMainMenu();
			message->SetCloseAllTextBox();
			message->SetCloseAllSelectBox();
		}
		else if (actionPressRelease == WidgetAction::enterMapEditor)
		{
			m_pState->SetGameStateMapEditor();
			m_pGUI->ChangeCurrentWindow(GUI::WINDOW::WindowIndex::WindowIndex_mapEditor);
			m_pMapEditor->SetInMap();
			// fali postavljanje kojeg tipa mape ! da render zna koju mapu crtati
			// hmm mozda ucitati mapu iz neke datoteke i ona odredjuje velicinu prije nego sto se unutar igre moze pisati ime i tip
			m_pMapEditor->InitCamera();
			message->SetInitMapEditor();
			message->SetCloseAllTextBox();
			message->SetCloseAllSelectBox();
		}
		else if (actionPressRelease == WidgetAction::enterMapEditorFromTiles) // entering map editor from an image selection window after NOT selecting an image
		{
			m_pGUI->ChangeCurrentWindow(GUI::WINDOW::WindowIndex::WindowIndex_mapEditor);
			m_pMapEditor->SetInMap();
			m_pMapEditor->InitCamera();
			message->SetInitMapEditor();
			message->SetCloseAllTextBox();
			message->SetCloseAllSelectBox();
			//message->SetClearSelectedTile();
		}
		else if (actionPressRelease == WidgetAction::enterMapEditorAllTiles || actionPressRelease == WidgetAction::enterMapEditorAllMonsters)
		{
			m_pGUI->ChangeCurrentWindow(GUI::WINDOW::WindowIndex_mapEditorImageSelection);

			if (actionPressRelease == WidgetAction::enterMapEditorAllTiles)
				m_pMapEditor->SetInAllTiles();
			else if (actionPressRelease == WidgetAction::enterMapEditorAllMonsters)
				m_pMapEditor->SetInAllMonsters();

			m_pMapEditor->InitCamera();
			message->SetInitMapEditorAllTiles();
			message->SetCloseAllTextBox();
			message->SetCloseAllSelectBox();
		}
		else if (actionPressRelease == WidgetAction::selectTile) // entering mapEditor from an image selection window after selecting an image
		{
			m_pMapEditor->SetSelectedTile();

			m_pGUI->ChangeCurrentWindow(GUI::WINDOW::WindowIndex::WindowIndex_mapEditor);
			m_pMapEditor->SetInMap();
			m_pMapEditor->InitCamera();

			message->SetInitMapEditor();
			//message->SetSelectedTile(); // not needed, just always update the RenderManager with selectedTileIndex from mapEditor upon entering MapEditorWindow
		}
		else if (actionPressRelease == WidgetAction::openChild)
		{
			if (!m_pGUI->OpenChildWidget(message))
				return	false;
		}
		else if (actionPressRelease == WidgetAction::saveMapEditorFile)
		{
			int n = m_pMapEditor->SaveSelectedMap();

			if (n == 0)
				ShowMessageBox("Save failed", "There's no created map");
			else if (n < 0)
				return	false;

			if (!m_pGUI->GetCurrentWindow()->AddMapToAllMaps(m_pMapEditor->GetFileName().c_str()))
				return	false;
		}
		else if (actionPressRelease == WidgetAction::loadMapEditorFile)
		{
			if (!m_pMapEditor->LoadMap(m_pGUI->GetCurrentWindow()->GetActiveSelectBox()->GetSelectedItem()))
				return	false;

			if (!m_pGUI->GetCurrentWindow()->CleanUpSelectBox())
				return	false;

			message->SetDestroyWidgets();
		}
		else if (actionPressRelease == WidgetAction::setMapEditorFileName)
		{
			std::string text = m_pGUI->GetCurrentWindow()->GetActiveTextBox()->GetText();

			m_pMapEditor->SetMapFileName(text);

			if (!m_pGUI->GetCurrentWindow()->CleanUpTextBox())
				return	false;

			message->SetStopTextInput();
			message->SetDestroyWidgets();
		}
		else if (actionPressRelease == WidgetAction::setMapEditorFileSize)
		{
			std::string text = m_pGUI->GetCurrentWindow()->GetActiveSelectBox()->GetSelectedItem();

			m_pMapEditor->SetMapFileSize(text);

			if (!m_pMapEditor->CreateNewMap())
				return	false;

			m_pGUI->GetCurrentWindow()->CleanUpSelectBox();
			message->SetDestroyWidgets();
		}
		else if (actionPressRelease == WidgetAction::textInput)
		{
			textInput = ClickedWidget::clickedWidget;
			//textInput = true;
		}
		else if (actionPressRelease == WidgetAction::selectBoxClicked)
		{
			selectBox = ClickedWidget::clickedWidget;
		}
	}

	if (textInput == ClickedWidget::clickedWidget)//(textInput)
	{
		m_pGUI->GetCurrentWindow()->StartTextInput();
		message->SetStartTextInput();
	}
	else if (textInput == ClickedWidget::clicked && m_pGUI->GetCurrentWindow()->ChildWidgetType() != WidgetType::textBox)
	{
		m_pGUI->GetCurrentWindow()->StopTextInput();
		message->SetStopTextInput();
	}

	if (selectBox == ClickedWidget::clickedWidget)
	{
		m_pGUI->GetCurrentWindow()->SetActiveSelectBox();
	}
	else if (selectBox == ClickedWidget::clicked && m_pGUI->GetCurrentWindow()->ChildWidgetType() != WidgetType::selectBox)
	{
		m_pGUI->GetCurrentWindow()->UnsetActiveSelectBox();
	}

	return	true;
}

bool CLogic::GetGUIAction(CMessage *message, CGUIAction *guiAction)
{
	const _inputMessage *InputMsg = message->GetInputMsg();

	const	_mouseEvents mouseEvent = InputMsg->mouse;
	const	int	mouseX = InputMsg->x;
	const	int	mouseY = InputMsg->y;
	const	int	mouseX1 = InputMsg->x1;
	const	int	mouseY1 = InputMsg->y1;
	const	int	mouseWheelY = InputMsg->y2;
	const	_keyboardEvents key = InputMsg->key;

	if (!m_pGUI->GetCurrentWindow()->ProcessInput(mouseEvent, key, mouseX, mouseY, mouseX1, mouseY1, mouseWheelY, guiAction))
		return	false;

	if (m_pState->IsGameStateInGame())
	{
		if (InputMsg->mouse.mouse == MouseEvents::LeftButton && InputMsg->mouse.state == InputState::released && !m_pEntityManager->GetPlayer()->GetEntity()->GetWeaponAttack()->IsAttackStarted() && mouseX > 0 && mouseX < g_windowWidth && mouseY > 0 && mouseY < g_windowHeight)
		{
			_mapPos	&pos{ m_pMap->GetPlayerMapPos() };

			m_pMovement->UpdateMapMovePosition(pos, static_cast<float>(InputMsg->x1), static_cast<float>(InputMsg->y1), m_pMap->GetCameraX(), m_pMap->GetCameraY());

			m_playerMovementByMouse = true;
		}
	}
	else if (m_pState->IsGameStateMainMenu())
	{

	}
	else if (m_pState->IsGameStateMapEditor())
	{
		if (m_pMapEditor->IsInMap())
		{
			// we check for pressed events ! because we want to insert tiles while the leftMouseButton is being pressed

			if (m_pGUI->GetCurrentWindow()->IsValidMapEditorSetTile(mouseX1, mouseY1))
			{
				if (mouseEvent.mouse == MouseEvents::LeftButton)
				{
					if (mouseEvent.state == InputState::pressed && m_pMapEditor->IsSelectedImageFloor())
					{
						m_pMapEditor->InsertTileToMap(mouseX1, mouseY1);
					}
					else if (mouseEvent.state == InputState::released)
					{
						m_pMapEditor->InsertTileToList(mouseX1, mouseY1);
					}
				}
				else if (mouseEvent.mouse == MouseEvents::RightButton && mouseEvent.state == InputState::released)
				{
					m_pMapEditor->RemoveTileFromList(mouseX1, mouseY1);
				}
			}
		}
		else if (m_pMapEditor->IsInImageWindow())
		{
			if (mouseEvent.mouse == MouseEvents::LeftButton && mouseEvent.state == InputState::released && mouseX > 0 && mouseX < g_windowWidth && mouseY > 0 && mouseY < g_windowHeight)
			{
				// a tile has been clicked, set it as a HUD tile ( it will be rendered onto HUD )
				// HUD on tile window always renders a HUD tile
				// because of that there's no need to send an event for the new tile to be rendered, it will be read from mapEditor when creating a new HUD image
				m_pMapEditor->SetHUDTile(mouseX, mouseY);
			}
		}
	}
	else if (m_pState->IsGameStateMainOptions())
	{

	}

	return	true;
}

void	CLogic::Movement(CMessage *message)
{
	const _inputMessage	*InputMsg = message->GetInputMsg();

	if (m_pState->IsGameStateInGame())
	{
		UpdateCamera(InputMsg->x1, InputMsg->y1, m_pMap);

		const int mapWidth = m_pMap->GetCurrentMapWidth();
		const int mapHeight = m_pMap->GetCurrentMapHeight();

		float playerX{ m_pMap->GetPlayerX() }, playerY{ m_pMap->GetPlayerY() };

		if ((InputMsg->leftRight.state == InputState::pressed || InputMsg->upDown.state == InputState::pressed) &&
			!m_pEntityManager->GetPlayer()->GetEntity()->GetWeaponAttack()->IsAttackStarted() && !m_waitingForAttackDirection)
		{
			_mapPos &playerMapPos{ m_pMap->GetPlayerMapPos() };

			float tMouseX{}, tMouseY{};
			float cameraX = m_pMap->GetCameraX(), cameraY = m_pMap->GetCameraY();
			float tPlayerX = playerX - cameraX + g_windowWidth / 2, tPlayerY = playerY - cameraY + g_windowHeight / 2;

			MapKeyboardMovementToMouse(tMouseX, tMouseY, InputMsg->leftRight, InputMsg->upDown, tPlayerX, tPlayerY);

			m_pMovement->UpdateMapMovePosition(playerMapPos, tMouseX, tMouseY, cameraX, cameraY);
			
			m_pMovement->MoveEntity(playerMapPos, m_deltaTime, mapWidth, mapHeight);
			
			m_playerMovementByMouse = false;
		}
		else if (m_playerMovementByMouse)
		{
			m_playerMovementByMouse = m_pMovement->MoveEntity(m_pMap->GetPlayerMapPos(), m_deltaTime, mapWidth, mapHeight);
		}
		
		std::vector<_mapPos> &enemyMapPos =  m_pEntityManager->GetEnemyMapPos();
		std::vector<_aiData> &enemyAIData = m_pEntityManager->GetAI()->GetAIData();

		for (size_t i = 0; i < enemyMapPos.size(); ++i)
		{
			if (!enemyAIData[i].path.empty())
			{
				m_pMovement->MoveEntity(enemyMapPos[i], enemyAIData[i].path, m_deltaTime);
			}
		}

		std::vector<int> cleanUpSpells{};
		
		m_pMovement->MoveSpells(m_pSpellManager->GetSpellMapPos(), m_deltaTime, mapWidth, mapHeight, cleanUpSpells);
		
		m_pSpellManager->DeleteSpells(cleanUpSpells);

		std::vector<int> cleanUpWeaponAttacks;

		m_pMovement->MoveWeaponAttacks(m_pEntityManager->GetCurrentWeaponAttacksForPlayer(), m_deltaTime, mapWidth, mapHeight, cleanUpWeaponAttacks);
	
		for (const auto& index : cleanUpWeaponAttacks)
			m_pEntityManager->DeleteCurrentPlayerWeaponAttack(index, true);

		cleanUpWeaponAttacks.clear();

		m_pMovement->MoveWeaponAttacks(m_pEntityManager->GetCurrentWeaponAttacksForEnemy(), m_deltaTime, mapWidth, mapHeight, cleanUpWeaponAttacks);

		for (const auto& index : cleanUpWeaponAttacks)
			m_pEntityManager->DeleteCurrentEnemyWeaponAttack(index, true);
	}
	else if (m_pState->IsGameStateMapEditor())
	{
		UpdateCamera(InputMsg->x1, InputMsg->y1, m_pMapEditor);
	}
}

void	CLogic::UpdateCamera(int mouseX, int mouseY, CMap *Map)
{
	float cameraX{ Map->GetCameraX() };
	float cameraY{ Map->GetCameraY() };

	m_pMovement->UpdateCamera(mouseX, mouseY, cameraX, cameraY, Map->GetCurrentMapWidth(), Map->GetCurrentMapHeight(), m_deltaTime);

	Map->SetCameraX(cameraX);
	Map->SetCameraY(cameraY);
}

bool	CLogic::Collision()
{
	if (!m_pState->IsGameStateInGame())
		return true;

	std::vector<std::pair<int, int>> playerCollOnEnemy;

	m_pCollision->CheckEntityCollision(&std::vector < _mapPos > {m_pMap->GetPlayerMapPosC()}, m_pEntityManager->GetEnemyMapPosC(), playerCollOnEnemy, CollisionType::PlayerEnemy);

	if (!playerCollOnEnemy.empty())
	{
		_mapPos &playerMapPos{ m_pMap->GetPlayerMapPos() };

		int x = static_cast<int>(playerMapPos.x + (playerMapPos.movX * 0.2 * playerMapPos.speed));
		int y = static_cast<int>(playerMapPos.y + (playerMapPos.movY * 0.2 * playerMapPos.speed));

		std::vector<_mapPos> &enemyMapPos{ m_pEntityManager->GetEnemyMapPos() };
		std::vector<_aiData> &aiData{ m_pEntityManager->GetAI()->GetAIData() };

		for (size_t i = 0; i < playerCollOnEnemy.size(); ++i)
		{
			if (CCollision::CheckCollision(x, y, static_cast<int>(enemyMapPos[playerCollOnEnemy[i].second].x), static_cast<int>(enemyMapPos[playerCollOnEnemy[i].second].y)))
			{
				playerMapPos.x = playerMapPos.previousX;
				playerMapPos.y = playerMapPos.previousY;
			}

			aiData[playerCollOnEnemy[i].second].path.clear();
			//enemyMapPos[playerCollOnEnemy[i].second].x = enemyMapPos[playerCollOnEnemy[i].second].previousX;
			//enemyMapPos[playerCollOnEnemy[i].second].y = enemyMapPos[playerCollOnEnemy[i].second].previousY;
		}
	}

	std::vector<std::pair<int, int>>	collidedSpells;

	m_pCollision->CheckEntityCollision(m_pSpellManager->GetSpellMapPos(), m_pEntityManager->GetEnemyMapPosC(), collidedSpells, CollisionType::SpellEnemy);

	if (!collidedSpells.empty())
		if (!SpellCollision(collidedSpells))
			return	false;

	std::vector<std::pair<int, int>>	collidedWeaponAttacks;

	m_pCollision->CheckWeaponAttackCollision(m_pEntityManager->GetCurrentWeaponAttacksForPlayer(), m_pEntityManager->GetEnemyMapPosC(), collidedWeaponAttacks, WeaponAttCollType::PlayerAttOnEnemy);

	if (!collidedWeaponAttacks.empty())
	{
		WeaponCollisionOnEnemy(collidedWeaponAttacks);
		collidedWeaponAttacks.clear();
	}

	m_pCollision->CheckWeaponAttackCollision(m_pEntityManager->GetCurrentWeaponAttacksForEnemy(), &std::vector < _mapPos > { m_pMap->GetPlayerMapPosC()}, collidedWeaponAttacks, WeaponAttCollType::EnemyAttOnPlayer);

	if (!collidedWeaponAttacks.empty())
	{
		WeaponCollisionOnPlayer(collidedWeaponAttacks);
	}

	return	true;
}

bool	CLogic::SpellCollision(std::vector<std::pair<int, int>> &collided)
{
	std::vector<int> spellIndex{};
	spellIndex.reserve(collided.size());

	for (const auto& index : collided)
	{
		CEntity *Enemy = m_pEntityManager->GetEnemy(index.second);

		if (Enemy != nullptr) // enemy could've died by another spell, so to not crash the game check for nullptr
		{
			int hp{ Enemy->GetHp() }, mp{ Enemy->GetMp() }, speed{ Enemy->GetSpeed() };

			m_pSpellManager->SpellHit(hp, mp, speed, index.first);

			if (hp > 0)
			{
				Enemy->SetHp(hp);
				Enemy->SetMp(mp);
				Enemy->SetSpeed(speed);
			}
			else
			{
				if (!m_pEntityManager->EnemyDied(index.second))
					return	false;
			}
		}

		spellIndex.push_back(index.first);
	}

	m_pSpellManager->DeleteSpells(spellIndex);

	return	true;
}

void	CLogic::WeaponCollisionOnEnemy(std::vector<std::pair<int, int>> &collided)
{
	// collided.first = attack index
	// collided.second = enemy ID

	for (auto& e : collided)
	{
		m_pEntityManager->PlayerAttackHit(e.first, e.second);
	}

	for (auto& e : collided)
	{
		m_pEntityManager->DeleteCurrentPlayerWeaponAttack(e.first);
	}
}

void	CLogic::WeaponCollisionOnPlayer(std::vector<std::pair<int, int>> &collided)
{
	for (auto& e : collided)
	{
		m_pEntityManager->EnemyAttackHit(e.first);
	}

	for (auto& e : collided)
	{
		m_pEntityManager->DeleteCurrentEnemyWeaponAttack(e.first);
	}
}

CMapEditor* CLogic::GetMapEditor()
{
	return	m_pMapEditor;
}

CMapInGame*	CLogic::GetMap()
{
	return	m_pMap;
}

CState* CLogic::GetState()
{
	return	m_pState;
}

CGUI*	CLogic::GetGUI()
{
	return	m_pGUI;
}

CSpellManager* CLogic::GetSpellManager()
{
	return	m_pSpellManager;
}

CPlayer*	CLogic::GetPlayer()
{
	return	m_pEntityManager->GetPlayer();
}

const CEntity*	CLogic::GetPlayerEntityC() const
{
	return	m_pEntityManager->GetPlayer()->GetEntityC();
}

CEntityManager* CLogic::GetEntityManager()
{
	return	m_pEntityManager;
}

void	CLogic::ReadMsgToLogic(CMessageToLogic *message)
{
	if (message->IsMsgToLogicEmpty()) // true means there's no msg, so we can return
		return;

	message->ClearMsgToLogic();
}

void	CLogic::MapMouseClickToKeys(const CMessage *Msg, _leftRight &lR, _upDown &uD)
{
	// there's already a key press caught or a mouse press/release hasn't been caught -> no need to map mouse 
	if (uD.state != InputState::nothing && lR.state != InputState::nothing || Msg->GetInputMsgC()->mouse.state == InputState::nothing) 
		return;

	int mouseX{ Msg->GetInputMsgC()->x1 }, mouseY{ Msg->GetInputMsgC()->y1 };
	int	playerX{ static_cast<int>(m_pMap->GetPlayerX()) }, playerY{ static_cast<int>(m_pMap->GetPlayerY()) };

	if (mouseX >= playerX && mouseX <= playerX + TILE_SIZE)
	{
		if (mouseY < playerY)
		{
			uD.upDown = UpDown::Up;
			uD.state = InputState::pressed;
		}
		else if (mouseY > playerY + TILE_SIZE)
		{
			uD.upDown = UpDown::Down;
			uD.state = InputState::pressed;
		}
	}
	else if (mouseX < playerX && mouseY >= playerY && mouseY <= playerY + TILE_SIZE)
	{
		lR.leftRight = LeftRight::Left;
		lR.state = InputState::pressed;
	}
	else if (mouseX > playerX + TILE_SIZE && mouseY >= playerY && mouseY <= playerY + TILE_SIZE)
	{
		lR.leftRight = LeftRight::Right;
		lR.state = InputState::pressed;
	}
}

void	CLogic::MapKeyboardMovementToMouse(float &mouseX, float &mouseY, const _leftRight &lR, const _upDown &uD, float playerX, float playerY)
{
	if (lR.leftRight == LeftRight::Left)
	{
		if (uD.upDown == UpDown::Up)
		{
			mouseX = playerX - 32;
			mouseY = playerY - 32;
		}
		else if (uD.upDown == UpDown::Down)
		{
			mouseX = playerX - 32;
			mouseY = playerY + 64;
		}
		else
		{
			mouseX = playerX - 32;
			mouseY = playerY + 16;
		}
	}
	else if (lR.leftRight == LeftRight::Right)
	{
		if (uD.upDown == UpDown::Up)
		{
			mouseX = playerX + 32;
			mouseY = playerY - 32;
		}
		else if (uD.upDown == UpDown::Down)
		{
			mouseX = playerX + 32;
			mouseY = playerY + 64;
		}
		else
		{
			mouseX = playerX + 32;
			mouseY = playerY + 16;
		}
	}
	else if (uD.upDown == UpDown::Up)
	{
		mouseX = playerX + 16;
		mouseY = playerY - 32;
	}
	else if (uD.upDown == UpDown::Down)
	{
		mouseX = playerX + 16;
		mouseY = playerY + 64;
	}

	if (mouseX < 0)
		mouseX = 0;

	if (mouseY < 0)
		mouseY = 0;

	if (mouseX > g_windowHeight)
		mouseY = static_cast<float>(g_windowHeight);

	if (mouseX > g_windowWidth)
		mouseX = static_cast<float>(g_windowWidth);
}
