#include <SDL2/SDL_timer.h> // for SDL_Delay and Uint32
#include "CGame.h" 
#include "Render/CRenderManager.h"
#include "Render/CRender.h"
#include "Input/CSDLInput.h" 
#include "Input/CSFMLInput.h"
#include "Logic/CLogic.h"
#include "Logic/State/CState.h"
#include "Logic/Map/CMap.h"
#include "Logic/Map/CMapEditor.h"
#include "Logic/Map/CMapInGame.h"
#include "Logic/Spell/CSpellManager.h"
#include "Logic/Entity/CEntityManager.h"
#include "Logic/Entity/Item/CItemManager.h"
#include "Logic/GUI/CGUI.h"
#include "Logic/GUI/CWindow.h"
#include "Logic/GUI/StructsGUI.h"
#include "Message/CMessage.h"
#include "Tools/CSDLTimer.h"
#include "Tools/ITimer.h"
#include "Tools/debug.h"
#include "Tools/Log.h"
#include "Tools/Functions.h"
#include "Tools/StringWrapper.h"
#include "Data/Globals.h"
#include "IO/IOFunctions.h"
#include "Data/Enums.h"

int		g_windowHeight = 640;
int		g_windowWidth = 1024;
int		g_windowHUDHeight = 64;

CGame::CGame()
{
	m_pRenderManager = nullptr;
	m_pLogic = nullptr;
	m_pInput = nullptr;
	m_pTimer = nullptr;
	m_pMessage = nullptr;

	m_seconds = 0;

	m_API = RenderAPI::nothing;
}

CGame::~CGame()
{

}

bool	CGame::Init()
{
	LoadConfig(); // for now we do not crash the game if there's no config file because we set the dafult values

	m_pRenderManager = DD_NEW CRenderManager;


	if (m_API == RenderAPI::SDL)
	{
		m_pInput = DD_NEW CSDLInput;
		m_pTimer = DD_NEW CSDLTimer;
	}
	else if (m_API == RenderAPI::SFML)
	{
		m_pInput = DD_NEW CSFMLInput;
		m_pTimer = DD_NEW CSDLTimer; // for now let the timer be sdl timer 
	}

	m_pLogic = DD_NEW CLogic; 
	m_pMessage = DD_NEW CMessage;

	if (m_pRenderManager->Init(m_API) == false)
	{
		Log("Render Manager Init failed");
		return	false;
	}

	if (m_API == RenderAPI::SFML) // SFML needs window for events
		m_pInput->VSetWindow(m_pRenderManager->GetRenderWindow());

	if (m_pMessage->Init() == false) // nobody should set a message before Init is called !
	{
		Log("Message Init failed");
		return	false;
	}


	if (m_pInput->VInit() == false)
	{
		Log("Input Init failed");
		return	false;
	}

	if (m_pLogic->Init(m_API) == false)
	{
		Log("Logic Init failed");
		return	false;
	}

	return	true;
}

void	CGame::Clean()
{
	if (m_pRenderManager != nullptr)
		m_pRenderManager->Clean();
	if (m_pLogic != nullptr)
		m_pLogic->Clean();
	if (m_pMessage != nullptr)
		m_pMessage->Clean();
	if (m_pInput != nullptr)
		m_pInput->VClean();

	DD_DELETE(m_pRenderManager);
	DD_DELETE(m_pTimer);
	DD_DELETE(m_pLogic);
	DD_DELETE(m_pInput);
	DD_DELETE(m_pMessage);

	DD::IO::Clean();
}

bool	CGame::RunGameLoop()
{
	bool	gameLoop = true;

	m_pMessage->SetInitMainMenu(); // message to render

	m_pLogic->GetGUI()->ChangeCurrentWindow(GUI::WINDOW::WindowIndex_mainMenu);

	double deltaTime = 0;
	double fps = 0;
	double currentTime = 0;

	m_pTimer->Start();
	double vSyncLastTime = m_pTimer->GetTimeEx();

	while (gameLoop)
	{
		currentTime = m_pTimer->GetTimeEx();

		if (Input() == false)
			return	true;

		m_pTimer->End();
		deltaTime = m_pTimer->GetTime();
		m_seconds += deltaTime;
		//fps = m_pTimer->GetMicro(); 
		m_pTimer->Start();

		if (m_pLogic->UpdateLogic(deltaTime, m_pMessage) == false)
			return	true;

		m_pRenderManager->UpdateTime(deltaTime);

		if (CGlobal::IsLimitFPS())
		{
			if (vSyncLastTime + 8333.333333 <= currentTime)
			{
				fps = currentTime - vSyncLastTime;
				vSyncLastTime = currentTime;

				m_pRenderManager->UpdateFrameRate(int(1000000 / fps));

				if (!Render(m_pMessage->GetInputMsg()->x1, m_pMessage->GetInputMsg()->y1))
					return	false;
			}
			else if (CGlobal::IsLimitCPU())
			{
				//Sleep(DWORD(vSyncLastTime + 8333.333333 - currentTime) / 1000);
				SDL_Delay((Uint32)(vSyncLastTime + 8333.333333 - currentTime) / 1000);
			}
		}
		else
		{
			fps = currentTime - vSyncLastTime;
			vSyncLastTime = currentTime;

			m_pRenderManager->UpdateFrameRate(int(1000000 / fps));

			if (!Render(m_pMessage->GetInputMsg()->x1, m_pMessage->GetInputMsg()->y1))
				return	false;
		}
	}

	return	true;
}

bool	CGame::Render(int mouseX,int mouseY)
{
	if (m_pMessage->IsMsgToRenderEmpty() == false) // returns true for MsgToRender being equal to MsgToRender::nothing
	{
		if (!ReadMsgToRender())
			return	false;
	}

	m_pRenderManager->GetRender()->VClearWindow();

	const CState *State = m_pLogic->GetState();

	if (State->IsGameStateInGame())
	{
		if (!RenderInGame(mouseX, mouseY))
			return	false;
	}
	else if (State->IsGameStateMainMenu())
	{
		m_pRenderManager->RenderMainMenu();
		m_pRenderManager->CreateHUD(mouseX, mouseY, (int)m_seconds);
	}
	else if (State->IsGameStateMainOptions())
	{
		m_pRenderManager->RenderMainOptions();
		m_pRenderManager->CreateHUD(mouseX, mouseY, (int)m_seconds);
	}
	else if (State->IsGameStateMapEditor())
	{
		if (!RenderMapEditor(mouseX, mouseY))
			return	false;
	}

	m_pRenderManager->RenderHUD();
	
#if defined(DD_ADDITIONAL_INFO)
	if (m_pLogic->GetState()->IsGameStateInGame())
	{
		m_pRenderManager->GetRender()->VRenderLine(std::string{ DD::GetPreciseStr(m_pLogic->GetMap()->GetPlayerX(), 2) + " " + DD::GetPreciseStr(m_pLogic->GetMap()->GetPlayerX(), 2) }.c_str(), 0, g_windowHeight + 2, 100, 100, 100, FontSize::font12, FontType::LiberationMono_Regular, TextFlag::center, 0, g_windowWidth);
		m_pRenderManager->GetRender()->VRenderLine(std::string{ DD::GetPreciseStr(m_pLogic->GetMap()->GetCameraX(), 2) + " " + DD::GetPreciseStr(m_pLogic->GetMap()->GetCameraY(), 2) }.c_str(), 0, g_windowHeight + 12, 100, 100, 100, FontSize::font12, FontType::LiberationMono_Regular, TextFlag::center, 0, g_windowWidth);
	}
#endif

	m_pRenderManager->GetRender()->VDrawWindow();

	return	true;
}

bool	CGame::RenderInGame(int mouseX, int mouseY)
{
	CMapInGame *Map = m_pLogic->GetMap();
	CEntityManager *EntityMng = m_pLogic->GetEntityManager();

	const int cameraX = static_cast<int>(Map->GetCameraX());
	const int cameraY = static_cast<int>(Map->GetCameraY());

	if (Map->IsMapShown())
	{
		const int playerX = static_cast<int>(Map->GetPlayerX());
		const int playerY = static_cast<int>(Map->GetPlayerY());
		const int mapWidth = Map->GetCurrentMapWidth();
		const int mapHeight = Map->GetCurrentMapHeight();

		m_pRenderManager->GetRender()->VRenderGameMap(cameraX, cameraY);
		m_pRenderManager->RenderVisibleEntities(EntityMng->GetEnemyMapPosC(), TYPE_IMG_MON, cameraX, cameraY, mapWidth, mapHeight);
		m_pRenderManager->RenderVisibleEntities(m_pLogic->GetSpellManager()->GetSpellMapPos(), TYPE_IMG_SPELL, cameraX, cameraY, mapWidth, mapHeight);
		m_pRenderManager->RenderPlayer(playerX, playerY, cameraX, cameraY, mapWidth, mapHeight);
		m_pRenderManager->RenderWeaponAttacks(EntityMng->GetCurrentWeaponAttacksForPlayer(), cameraX, cameraY, mapWidth, mapHeight);
		m_pRenderManager->RenderWeaponAttacks(EntityMng->GetCurrentWeaponAttacksForEnemy(), cameraX, cameraY, mapWidth, mapHeight);
	}

	m_pRenderManager->CreateInGameHUD(mouseX, mouseY, static_cast<int>(m_seconds), EntityMng->GetPlayer(), EntityMng->GetInGameHoverData());
	
	return	true;
}

bool	CGame::RenderMapEditor(int mouseX,int mouseY)
{
	CMapEditor *p = m_pLogic->GetMapEditor();

	const int cameraX = static_cast<int>(p->GetCameraX());
	const int cameraY = static_cast<int>(p->GetCameraY());

	if (p->IsInMap())
	{
		if (p->IsMapSmall())
		{
			m_pRenderManager->RenderMap(p->GetMapSmall(),cameraX,cameraY);
			m_pRenderManager->RenderVisibleEntities(m_pLogic->GetMapEditor()->GetEnemyMapPos(), TYPE_IMG_MON, cameraX, cameraY, MAP_WIDTH_SMALL, MAP_HEIGHT_SMALL);
		}
		else if (p->IsMapMedium())
		{
			m_pRenderManager->RenderMap(p->GetMapMedium(), cameraX, cameraY);
			m_pRenderManager->RenderVisibleEntities(m_pLogic->GetMapEditor()->GetEnemyMapPos(), TYPE_IMG_MON, cameraX, cameraY, MAP_WIDTH_MEDIUM, MAP_HEIGHT_MEDIUM);
		}
		else if (p->IsMapBig())
		{
			m_pRenderManager->RenderMap(p->GetMapBig(), cameraX,cameraY);
			m_pRenderManager->RenderVisibleEntities(m_pLogic->GetMapEditor()->GetEnemyMapPos(), TYPE_IMG_MON, cameraX, cameraY, MAP_WIDTH_BIG, MAP_HEIGHT_BIG);
		}

		m_pRenderManager->CreateHUD(mouseX, mouseY, (int)m_seconds);

	}
	else if (p->IsInImageWindow())
	{
		const	int	imgType = p->GetCurrentImgType();

		if (imgType < 0)
		{
			Log("CGame::RenderMapEditor() failed");
			return	false;
		}

		m_pRenderManager->RenderAllImg(p->GetCurrentImgMap(), imgType, (int)p->GetCameraX(), (int)p->GetCameraY());

		m_pRenderManager->CreateImgSelectionHUD(mouseX, mouseY, (int)m_seconds, m_pLogic->GetMapEditor()->GetHUDTile(), imgType);
	}

	return	true;
}

bool	CGame::ReadMsgToRender()
{
	// one can afford these checks because they don't happen often

	if (m_pLogic->GetState()->IsGameStateInGame())
	{
		if (m_pMessage->IsNewMapInGame())
		{
			m_pRenderManager->GetRender()->VCreateGameMap(m_pLogic->GetMap()->GetCurrentMap());
		}
		else if (m_pMessage->IsStartWeaponMainAnimation())
		{

		}

		if (m_pMessage->IsSetInGameHUDSpellData())
		{
			m_pRenderManager->CreateInGameHUDData(m_pLogic->GetSpellManager()->GetPlayerHudSpells(), RenderHudType::spells);
		}

		if (m_pMessage->IsSetInGameHUDWeapAttData())
		{
			m_pRenderManager->CreateInGameHUDData(m_pLogic->GetEntityManager()->GetItemManager()->GetPlayerHUDWeapAtt(), RenderHudType::weaponAttacks);
		}
	}

	if (m_pMessage->IsSetDestroyWidgets())
	{
		if (!m_pRenderManager->DestroyWidgets(m_pLogic->GetGUI()->GetCurrentWindow()->GetDestroyWidgetData()))
			return	false;

		m_pLogic->GetGUI()->GetCurrentWindow()->ClearDestroyWidgetData();
	}

	if (m_pMessage->IsInitInGame())
	{
		m_pRenderManager->GetRender()->VCleanButtonImages();
		m_pRenderManager->PrepareButtons(&m_pLogic->GetGUI()->GetCurrentWindow()->GetButtons()->button);
	}
	else if (m_pMessage->IsInitMainMenu())
	{
		m_pRenderManager->GetRender()->VCleanButtonImages();
		m_pRenderManager->PrepareButtons(&m_pLogic->GetGUI()->GetCurrentWindow()->GetButtons()->button);
	}
	else if (m_pMessage->IsInitMapEditor())
	{
		// fali postavljanje indexa is GetSelectedImgIndex neki vrag iz tile-a
		// oops, renderu ne treba selectedImgIndex... ako se dogodi click na mapEditor mapi to logika treba sredjivati
		// citati koji je odabran tile te ga staviti na mapi a render samo cita mapu
		m_pRenderManager->GetRender()->VCleanButtonImages();
		m_pRenderManager->PrepareButtons(&m_pLogic->GetGUI()->GetCurrentWindow()->GetButtons()->button);
	}
	else if (m_pMessage->IsInitMapEditorAllTiles())
	{
		m_pRenderManager->GetRender()->VCleanButtonImages();
		m_pRenderManager->PrepareButtons(&m_pLogic->GetGUI()->GetCurrentWindow()->GetButtons()->button);
	}
	
	if (m_pMessage->IsCreateButtonMenu())
	{
		m_pRenderManager->PrepareButtons(&m_pLogic->GetGUI()->GetCurrentWindow()->GetAllButtonMenu()->menu);
	}
	
	if (m_pMessage->IsCreateTextBox())
	{
		m_pRenderManager->CreateTextBox(m_pLogic->GetGUI()->GetCurrentWindow()->GetActiveTextBox());
	}

	if (m_pMessage->IsCreateSelectBox())
	{
		m_pRenderManager->CreateSelectBox(m_pLogic->GetGUI()->GetCurrentWindow()->GetActiveSelectBox());
	}
	 
	if (m_pMessage->IsCloseAllTextBox())
	{
		m_pRenderManager->GetRender()->VCleanAllTextInputImages();
	}

	if (m_pMessage->IsCloseAllSelectBox())
	{
		m_pRenderManager->GetRender()->VCleanAllSelectBoxImages();
	}
	
	m_pMessage->ClearMsgToRender();

	return	true;
}

bool	CGame::LoadConfig()
{
	return	DD::IO::LoadConfigFile(m_API);
}

bool	CGame::Input()
{
	if (m_pMessage->IsMsgToInputEmpty() == false)
	{
		if (m_pMessage->IsStartTextInput())
		{
			m_pInput->VStartTextInput();
		}
		else if (m_pMessage->IsStopTextInput())
		{
			m_pInput->VStopTextInput();
		}

		m_pMessage->ClearMsgToInput();
	}

	m_pInput->VUpdateInputMsg(m_pMessage->GetInputMsg()); // get player input

	if (m_pInput->IsOver())
		return false;

	return	true;
}
