#pragma once
#include "../Data/Const.h"

#include <string>
#include <vector>

enum class ColorIndex;

struct _mapTileAll;
struct _inGameHoverData;

namespace sf
{
	class RenderWindow;
}

struct _renderButtonData
{
	char	text[BUTTON_TEXT_SIZE];
	int		x, y, w, h;
	int		r1, g1, b1; // box color
	int		r2, g2, b2; // text color
	int		id;
};

struct _renderSpellData
{
	float	x, y;
	int		index;
	int		id;
	int		speed;
	double	duration;
};

namespace TextFlag
{

	enum  TextFlag
	{
		nothing = 0x01,
		align = 0x02, 
		alignLeft = 0x04, 
		alignRight = 0x08,
		center = 0x10
	};
}

enum class FontType
{
	nothing,
	LiberationMono_Regular
};

enum class ImgTextureType
{
	nothing,
	button,
	textBoxInput
};

enum class RenderHudType
{
	weaponAttacks, spells
};

enum class FontSize;

class CRender
{
private:

protected:

	std::vector<_renderSpellData>	m_spells;

public:

	CRender();
	virtual ~CRender(){}

	virtual bool	VInit() = 0;
	virtual void	VClean() = 0;

public:

	virtual	void	VClearWindow() = 0;
	virtual void	VDrawWindow() = 0;

	virtual void	VRenderLine(const char* const text, int x, const int y, const ColorIndex &colorIndex,FontSize fontSize,FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) = 0;
	virtual void	VRenderLine(const char* const text, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) = 0;

	virtual void	VRenderValue(const int value, int x, const int y, const ColorIndex &colorIndex, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) = 0;
	virtual void	VRenderValue(const int value, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) = 0;

	virtual void	VRenderImage(const int x, const int y, const int w,const int h, const int imgIndex,const int imgType) = 0;
	virtual void	VRenderImage(const int x, const int y, const int imgIndex, const int imgType) = 0;
	virtual void	VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType, double angle) = 0;

	virtual void	VRenderButton(const char *text, int x, int y, int r, int g, int b) = 0;
	// renders buttons set by VSetButtonData
	virtual void	VRenderAllButtons() = 0;
	// creates a texture and stores it, VCleanButtonImages must be called when we do not need the texture anymore ( before new call to this function )
	virtual void	VCreateButtonImage(_renderButtonData *data) = 0;
	virtual void	VCleanButtonImages() = 0;
	// returns false if there's no such button image, writes to log
	virtual bool	VCleanButtonImage(int id) = 0;

	virtual void	VCleanAllSelectBoxImages() = 0;
	// returns false if there's no such select box image, writes to log
	virtual	bool	VCleanSelectBoxImage(int id) = 0;
	virtual void	VRenderAllSelectBox() = 0;
	virtual void	VCreateSelectBoxImage(int id, const std::vector<std::string> &text, int x, int y, int startY, int sizeY, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) = 0;

	// creates a text box for input image
	// r1,g1,b1 - text box color 
	// r2,g2,b2 - text color
	virtual void	VCreateTextInputImage(int id, char *title, const std::string &text, int x, int y, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) = 0;
	virtual	void	VCleanAllTextInputImages() = 0;
	// returns false if there's no text input with sent id ( error )
	// writes to log
	virtual	bool	VCleanTextInputImage(int id) = 0;

	// if there's a textinput render it
	// so, to stop rendering textinputs delete them from renderer
	virtual	void	VRenderAllTextInputs() = 0;

	// renders an empty image for HUD
	virtual void	VRenderHUD() = 0;
	// it will destroy the image if there's already one and create a new one
	// set callDisplay to true to call the display() method on SFML version and on SDL to set to default render target, and false to not call it and on SDL to the hud will be the render target
	virtual void	VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY,bool callDisplay = true) = 0;
	// it will destroy the image if there's already one and create a new one
	virtual void	VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, int imgIndex, int imgType) = 0;
	virtual void	VCreateInGameHUDImage(int frameRate, int sec, int mouseX, int mouseY, int hp, int mp) = 0;

	virtual void	VCreateInGameHoverData(const _inGameHoverData *hoverData) = 0;
	virtual void	VCreateInGameHUDData(RenderHudType hudType, const std::vector<int> &imgIndex, int x, int y, int w, int h, int offsetX) = 0;

	virtual	void	VCreateGameMap(const _mapTileAll *map) = 0;
	virtual	void	VRenderGameMap(const int cameraX, const int cameraY) = 0;

	// x will be updated ( ), x1 can be left or right border and x2 should be sent if we want to center ( then x1 is left and x2 is right border )
	// this function will not check if align is set to nothing !, do that outside of the function !
	// ( for leftAlign x = x1, for rightAlign x = x1-width.... so for x1 put some offset (increase the value by 8 or by a different value) )
	void			AlignmentX(TextFlag::TextFlag flag, int &x, int x1, int width = -1, int x2 = -1);


	// returns false if there's already an imgTexture  with imgTexture.id = id_
	// returns true if there's no imgTexture with such id
	virtual bool	VValidId(int id_, const ImgTextureType &type) = 0;

	// CSFMLRender 
	virtual	sf::RenderWindow*	VGetRenderWindow() { return nullptr; }

	// pass map position as x and y, they will be updated with window positions if x and y presented are visible 
	// ( only a part needs to be visible on the window )
	bool	CheckIfInWindow(int cameraX, int cameraY, int mapWidth, int mapHeight, int &x, int &y);
};
