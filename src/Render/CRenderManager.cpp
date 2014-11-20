#include "CRenderManager.h"
#include "CSDLRender.h"
#include "CSFMLRender.h"
#include "../Tools/debug.h"
#include "../Tools/Log.h"
#include "../Tools/StringWrapper.h"
#include "../Data/Enums.h"
#include "../Data/Globals.h"
#include "../Logic/GUI/CMenu.h"
#include "../Logic/GUI/CTextBox.h"
#include "../Logic/GUI/StructsGUI.h"
#include "../Logic/Entity/CPlayer.h"
#include "../Logic/Entity/EntityWrapper.h"
#include "../Logic/Entity/Item/ItemEnums.h"

CRenderManager::CRenderManager()
{
	m_pRender = nullptr;
	m_renderAPI = RenderAPI::nothing;
}

CRenderManager::~CRenderManager()
{

}

bool	CRenderManager::Init(const RenderAPI &api)
{
	m_renderAPI = api;

	if (m_renderAPI == RenderAPI::SDL)
	{
		m_pRender = DD_NEW CSDLRender;
	}
	else if (m_renderAPI == RenderAPI::SFML)
	{
		 m_pRender = DD_NEW CSFMLRender;
	}
	else
	{
		Log("Render API not selected for CRender");
		return	false;
	}


	if (m_pRender->VInit() == false)
	{
		Log("Render Init failed");
		return	false;
	}

	m_timerHUD = 0;

	for (int i = 0; i < m_FRAME_RATE_SIZE; i++)
		m_frameRate[i] = 0;

	m_frameRateIndex = 0;

	return	true;
}

void	CRenderManager::Clean()
{
	if (m_pRender != nullptr)
		m_pRender->VClean();

	DD_DELETE(m_pRender);
}

void	CRenderManager::PrepareButtons(std::vector<CMenu> *menu)
{
	for (std::vector<CMenu>::iterator it = menu->begin(); it != menu->end(); ++it)
	{
		if (it->IsVisible())
		{
			PrepareButtons(it->GetButtons());
		}
	}
}

bool	CRenderManager::DestroyWidgets(_GUITypeIdWrapper *widgets_)
{
	for (size_t i = 0; i < widgets_->GUITypeId.size(); i++)
	{
		if (widgets_->GUITypeId[i].type == WidgetType::textBox)
		{
			if (!m_pRender->VCleanTextInputImage(widgets_->GUITypeId[i].id))
				return	false;
		}
		else if (widgets_->GUITypeId[i].type == WidgetType::button)
		{
			if (!m_pRender->VCleanButtonImage(widgets_->GUITypeId[i].id))
				return	false;
		}
		else if (widgets_->GUITypeId[i].type == WidgetType::selectBox)
		{
			if (!m_pRender->VCleanSelectBoxImage(widgets_->GUITypeId[i].id))
				return	false;
		}
		else
		{
			Log("CRenderManager::DestroyWidgets() failed, wrong type");
			return	false;
		}
	}

	return	true;
}

void	CRenderManager::PrepareButtons(std::vector<CButton> *buttons)
{
	_renderButtonData	data{};

	const	int	textSize{ sizeof(data.text) };

	data.r1 = 255, data.g1 = 255, data.b1 = 255;
	data.r2 = 70, data.g2 = 120, data.b2 = 70;
	
	for (std::vector<CButton>::iterator it = buttons->begin(); it != buttons->end(); ++it)
	{
		if (m_pRender->VValidId(it->GetWidgetID(), ImgTextureType::button)) // if there's no such button, create it
		{
			DD_Strcpy(data.text, textSize, it->GetText());

			data.w = it->GetW();
			data.h = it->GetH();
			data.x = it->GetX();
			data.y = it->GetY();
			data.id = it->GetWidgetID();

			m_pRender->VCreateButtonImage(&data);
		}
	}
}

void	CRenderManager::CreateTextBox(CTextBox *textBox)
{
	m_pRender->VCreateTextInputImage(textBox->GetWidgetID(),textBox->GetLabel(), textBox->GetText(), textBox->GetX(), textBox->GetY(), textBox->GetW(), textBox->GetH(), textBox->GetR1(), textBox->GetG1(), textBox->GetB1(),
		textBox->GetR2(), textBox->GetG2(), textBox->GetB2(), FontSize::font16);
}

void	CRenderManager::CreateSelectBox(CSelectBox *selectBox)
{
	m_pRender->VCreateSelectBoxImage(selectBox->GetWidgetID(), selectBox->GetVisibleItems(), selectBox->GetX(), selectBox->GetY(), selectBox->GetItemStartY(), selectBox->GetItemSizeY(), selectBox->GetW(), selectBox->GetH(),
		selectBox->GetR1(), selectBox->GetG1(), selectBox->GetB1(), selectBox->GetR2(), selectBox->GetG2(), selectBox->GetB2(), selectBox->GetFontSize(), FontType::LiberationMono_Regular, TextFlag::center, 0, selectBox->GetW());
}

void	CRenderManager::RenderMainMenu()
{

}

void	CRenderManager::RenderMainOptions()
{

}

void	CRenderManager::RenderMap(const _mapTileSmall *map,const	int cameraX_,const int cameraY_)
{
	int mapX = cameraX_ - g_windowWidth / 2;
	int mapY = cameraY_ - g_windowHeight / 2;

	int downX = (mapX / TILE_SIZE) * TILE_SIZE;
	int downY = (mapY / TILE_SIZE)*TILE_SIZE;

	for (int y = downY - mapY; y < g_windowHeight; y += TILE_SIZE)
	{
		for (int x = downX - mapX; x < g_windowWidth; x += TILE_SIZE)
		{
			m_pRender->VRenderImage(x, y, TILE_SIZE, TILE_SIZE, map->map[mapY / 32][mapX / 32].imgIndex, TYPE_IMG_FLOOR);
			mapX += TILE_SIZE;
		}

		mapY += TILE_SIZE;
		mapX = cameraX_ - g_windowWidth / 2;
	}
}

void	CRenderManager::RenderMap(const _mapTileMedium *map, const  int cameraX_, const int cameraY_)
{
	int mapX = cameraX_ - g_windowWidth / 2;
	int mapY = cameraY_ - g_windowHeight / 2;

	int downX = (mapX / TILE_SIZE) * TILE_SIZE;
	int downY = (mapY / TILE_SIZE)*TILE_SIZE;

	for (int y = downY - mapY; y < g_windowHeight; y += TILE_SIZE)
	{
		for (int x = downX - mapX; x < g_windowWidth; x += TILE_SIZE)
		{
			m_pRender->VRenderImage(x, y, TILE_SIZE, TILE_SIZE, map->map[mapY / 32][mapX / 32].imgIndex, TYPE_IMG_FLOOR);
			mapX += TILE_SIZE;
		}

		mapY += TILE_SIZE;
		mapX = cameraX_ - g_windowWidth / 2;
	}
}

void	CRenderManager::RenderMap(const _mapTileBig *map, const  int cameraX_, const int cameraY_)
{
	int mapX = cameraX_ - g_windowWidth / 2;
	int mapY = cameraY_ - g_windowHeight / 2;

	int downX = (mapX / TILE_SIZE) * TILE_SIZE;
	int downY = (mapY / TILE_SIZE) * TILE_SIZE;

	for (int y = downY - mapY; y < g_windowHeight; y += TILE_SIZE)
	{
		for (int x = downX - mapX; x < g_windowWidth; x += TILE_SIZE)
		{
			m_pRender->VRenderImage(x, y, TILE_SIZE, TILE_SIZE, map->map[mapY / 32][mapX / 32].imgIndex, TYPE_IMG_FLOOR);
			mapX += TILE_SIZE;
		}

		mapY += TILE_SIZE;
		mapX = cameraX_ - g_windowWidth / 2;
	}
}

void	CRenderManager::RenderVisibleEntities(const std::vector<_mapPos> *entities,const int imgType, const int cameraX, const int cameraY, const int mapWidth, const int mapHeight)
{
	for (const auto& entity : *entities)
	{
		int tmpX{ static_cast<int>(entity.x) };
		int tmpY{ static_cast<int>(entity.y) };

		if (m_pRender->CheckIfInWindow(cameraX, cameraY, mapWidth, mapHeight, tmpX, tmpY))
		{
			m_pRender->VRenderImage(tmpX, tmpY, entity.index, imgType);
		}
	}
}

void	CRenderManager::RenderPlayer(const int playerX, const int playerY, const int cameraX, const int cameraY, const int mapWidth, const int mapHeight)
{
	int tmpX{ playerX }, tmpY{ playerY };

	if (m_pRender->CheckIfInWindow(cameraX, cameraY, mapWidth, mapHeight, tmpX, tmpY))
	{
		m_pRender->VRenderImage(tmpX, tmpY, 0, TYPE_IMG_PLAYER);
	}
}

void	CRenderManager::RenderWeaponAttacks(const _weaponAttackPosWrapper *pos, const int cameraX, const int cameraY, const int mapWidth, const int mapHeight)
{
	for (const auto& attack : pos->pos)
	{
		int tmpX{ static_cast<int>(attack.posX) }, tmpY{ static_cast<int>(attack.posY) };

		if (m_pRender->CheckIfInWindow(cameraX, cameraY, mapWidth, mapHeight, tmpX, tmpY))
		{
			if (attack.attackType == WeaponAttackType::normalSlash)
				m_pRender->VRenderImage(tmpX, tmpY, 32, 32, attack.imgIndex, TYPE_IMG_ITEM, attack.currentValue);
		}
	}
}

void	CRenderManager::RenderAllImg(const _mapEditorImg *map, const int imgType, const int cameraX_, const int cameraY_)
{
	int	mapX = cameraX_ - g_windowWidth / 2;
	int mapY = cameraY_ - g_windowHeight / 2;

	int	downX = (mapX / TILE_SIZE) * TILE_SIZE;
	int downY = (mapY / TILE_SIZE) * TILE_SIZE;

	for (int y = downY - mapY; y < g_windowHeight; y += TILE_SIZE)
	{
		for (int x = downX - mapX; x < g_windowWidth; x += TILE_SIZE)
		{
			m_pRender->VRenderImage(x, y,TILE_SIZE,TILE_SIZE, map->map[mapY / 32][mapX / 32], imgType);
			mapX += TILE_SIZE;
		}

		mapY += TILE_SIZE;
		mapX = cameraX_ - g_windowWidth / 2;
	}
}

void	CRenderManager::RenderHUD()
{
	m_pRender->VRenderHUD();

	m_pRender->VRenderAllButtons();
	m_pRender->VRenderAllTextInputs();
	m_pRender->VRenderAllSelectBox();
}

void	CRenderManager::CreateHUD(int mouseX, int mouseY, int sec)
{
	if (m_timerHUD <= 0)
	{
		m_timerHUD = m_TIMER_HUD;
		m_pRender->VCreateHUDImage(GetAvgFrameRate(), sec, mouseX, mouseY);
	}
}

void	CRenderManager::CreateImgSelectionHUD(int mouseX, int mouseY,int sec, int imgIndex, int imgType)
{
	if (m_timerHUD <= 0)
	{
		m_timerHUD = m_TIMER_HUD;
		m_pRender->VCreateHUDImage(GetAvgFrameRate(), sec, mouseX, mouseY, imgIndex, imgType);
	}
}

void	CRenderManager::CreateInGameHUD(int mouseX, int mouseY, int sec, const CPlayer *Player, const _inGameHoverData *hoverData)
{
	if (m_timerHUD <= 0)
	{
		m_timerHUD = m_TIMER_HUD;
		
		m_pRender->VCreateInGameHUDImage(GetAvgFrameRate(), sec, mouseX, mouseY, Player->GetEntityC()->GetHp(), Player->GetEntityC()->GetMp());
		
		m_pRender->VCreateInGameHoverData(hoverData);
	}
}

void	CRenderManager::CreateInGameHUDData(const _inGameHudData *data, RenderHudType hudType)
{
	m_pRender->VCreateInGameHUDData(hudType, data->index, data->x, data->y, data->w, data->h, data->offsetX);
}

void	CRenderManager::UpdateTime(double time)
{
	m_timerHUD -= time;
}

void	CRenderManager::UpdateFrameRate(int frameRate)
{
	m_frameRate[m_frameRateIndex] = frameRate;

	m_frameRateIndex++;

	if (m_frameRateIndex == m_FRAME_RATE_SIZE) // if we've reached the array size, reset the index
		m_frameRateIndex = 0;
}

int		CRenderManager::GetAvgFrameRate()
{
	int sum = 0;

	for (int i = 0; i < m_FRAME_RATE_SIZE; i++)
	{
		sum += m_frameRate[i];
	}

	return	(int)sum / m_FRAME_RATE_SIZE;
}

CRender*	CRenderManager::GetRender()
{
	return	m_pRender;
}

sf::RenderWindow*	CRenderManager::GetRenderWindow()
{
	if (m_renderAPI == RenderAPI::SFML)
	{
		return	m_pRender->VGetRenderWindow();
	}

	return	nullptr;
}
