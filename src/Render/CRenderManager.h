#pragma once

#include <vector>

class CRender;
class CButton;
class CTextBox;
class CSelectBox;
class CMenu;
class CPlayer;

struct _mapTileSmall;
struct _mapTileMedium;
struct _mapTileBig;
struct _mapEditorImg;
struct  _GUITypeIdWrapper;
struct  _mapPos;
struct _weaponAttackPosWrapper;
struct _inGameHoverData;
struct _inGameHudData;

enum class RenderAPI;
enum class RenderHudType;

namespace sf
{
	class RenderWindow;
}

class CRenderManager
{
private:

	CRender *m_pRender;

	RenderAPI	m_renderAPI;

	const double m_TIMER_HUD = 0.083333333; // used for setting m_timerHUD value after it has reached zero
	double	m_timerHUD; // when it reaches zero (<=0) create a new HUDImage
	
	static const int m_FRAME_RATE_SIZE = 15;
	int		m_frameRateIndex; // m_frameRate[m_frameRateIndex] -> used for setting a value inside the array, when it reaches FRAME_RATE_SIZE-1 reset it
	int		m_frameRate[m_FRAME_RATE_SIZE]; // array used for storing frame rate

private:

	// calculates the avg frame rate ( uses values from m_frameRate[] set by UpdateTime )
	int		GetAvgFrameRate(); 

public:

	CRenderManager();
	~CRenderManager();

	bool	Init(const RenderAPI &api);
	void	Clean();

public:

	// reduce the value of each timer variable, other functions should check for <= 0
	// this function is not locked by g_limitFPS
	void	UpdateTime(double time);

	// updates the frameRate array ( used for getting AvgFrameRate -> which is used for frame rate on display)
	// will also take care of frameRateIndex 
	void	UpdateFrameRate(int frameRate);

	void	RenderMainMenu();

	void	CreateTextBox(CTextBox *textBox);

	void	CreateSelectBox(CSelectBox *selectBox);

	void	PrepareButtons(std::vector<CButton> *buttons);
	void	PrepareButtons(std::vector<CMenu> *menu);

	void	RenderMainOptions();

	void	RenderMap(const _mapTileSmall *map, const	int cameraX_, const int cameraY_);
	void	RenderMap(const _mapTileMedium *map, const int cameraX_, const int cameraY_);
	void	RenderMap(const _mapTileBig *map, const int cameraX_, const int cameraY_);

	void	RenderVisibleEntities(const std::vector<_mapPos> *entities,const int imgType ,const int cameraX,const int cameraY,const int mapWidth,const int mapHeight);
	void	RenderPlayer(const int playerX, const int playerY, const int cameraX, const int cameraY, const int mapWidth, const int mapHeight);

	void	RenderWeaponAttacks(const _weaponAttackPosWrapper *pos, const int cameraX, const int cameraY, const int mapWidth, const int mapHeight);

	void	RenderHUD();

	// creates an image 12 times per second
	void	CreateHUD(int mouseX, int mouseY, int sec);

	// creates an image 12 times per second
	void	CreateImgSelectionHUD(int mouseX, int mouseY,int sec, int imgIndex,int imgType);

	// creates an image 12 times per second
	void	CreateInGameHUD(int mouseX, int mouseY, int sec, const CPlayer *Player, const _inGameHoverData *hoverData);
	
	// creates the hud icons, each new call should be made upon hud change 
	void	CreateInGameHUDData(const _inGameHudData *data, RenderHudType hudType);

	// render every img available of type imgType
	void	RenderAllImg(const _mapEditorImg *map,const int imgType, const int cameraX_, const int cameraY_);

	// returns false on error
	bool	DestroyWidgets(_GUITypeIdWrapper *widgets_);

	CRender*	GetRender();

	// will return RenderWindow if renderAPI is sfml
	sf::RenderWindow* GetRenderWindow();
};
