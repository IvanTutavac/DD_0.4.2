#pragma once

#include <map>

#include "CRender.h"
#include "CSFMLImage.h"

namespace sf
{
	class RenderWindow;
	class RenderTexture;
	class Font;
	class Texture;
	class Sprite;
};

struct _imgTextureSFML
{
	int x, y, w, h;
	int r, g, b;
	int	id; // id of the image, used if one wants to delete a specific image
	sf::Texture	*texture;
	sf::Sprite	*sprite;
};

struct _imgRenderTextureSFMLReduced
{
	sf::RenderTexture *texture;
	sf::Sprite	*sprite;
};

typedef std::map<RenderHudType, _imgRenderTextureSFMLReduced>::iterator HudDataSFMLIterator;

class CSFMLRender : public CRender
{
private:

	sf::RenderWindow	*m_pWindow;
	sf::Font			*m_pFont;

	std::vector<CSFMLImage>	m_vImage;// m_vImage[TYPE_IMAGE].m_vTexture[IMG_INDEX].texture

	std::vector<_imgTextureSFML>	m_vButtonImage;
	std::vector<_imgTextureSFML>	m_vTextInputImage;
	std::vector<_imgTextureSFML>	m_vSelectBoxImage;

	sf::RenderTexture	*m_pHUDTexture;
	sf::Sprite			*m_pHUDSprite;

	sf::RenderTexture	*m_pMapTexture;
	sf::Sprite			*m_pMapSprite;

	sf::RenderTexture	*m_pHoverTexture;
	sf::Sprite			*m_pHoverSprite;
	bool				 m_hoverTextureReady;

	std::map<RenderHudType, _imgRenderTextureSFMLReduced>	m_hudData;

private:

	// loads images
	// returns false on error and writes to log
	bool	LoadImages();

	// loads fonts
	// returns false on error and writes to log
	bool	LoadFonts();

	// returns nullptr on error and writes to log
	sf::Font*	GetFont(const FontType &fontType);

	// for wrong fontSize 16 will be returned
	unsigned	int	GetFontSize(const	FontSize	&fontSize);

	// des -> destination texture where one wants to render the text, if nullptr, text will be rendered onto window
	void		RenderLine(sf::RenderTexture *des,const char *text, int x, const int y,sf::Font *Font,unsigned int FontSize, int r,int g,int b, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1);

	void		RenderImage(const int x, const int y, sf::Sprite *src);

	void		RenderImageEx(const int x, const int y, const int w, const int h, double angle, float originX, float originY, sf::Sprite *sprite);

	void		RenderImage(const int x, const int y, sf::RenderTexture *des,sf::Sprite *src);

	void		RenderImage(int x, int y, int imgIndex, int imgType, sf::RenderTexture *des);

	void		RenderImage(int srcX, int srcY, int srcW, int srcH, int desX, int desY, sf::RenderTexture *des, sf::Sprite *src);

	void		RenderImage(int srcX, int srcY, int srcW, int srcH, int desX, int desY, sf::Sprite *src);

public:

	CSFMLRender();
	~CSFMLRender();

	bool	VInit() override;
	void	VClean() override;

public:

	void	VClearWindow() override;
	void	VDrawWindow() override;

	void	VRenderLine(const char* const text, int x, const int y, const ColorIndex  &colorIndex, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;
	void	VRenderLine(const char* const text, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;

	void	VRenderValue(const int value, int x, const int y, const ColorIndex  &colorIndex, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;
	void	VRenderValue(const int value, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;

	void	VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType) override;
	void	VRenderImage(const int x, const int y, const int imgIndex, const int imgType) override;
	void	VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType, double angle) override;

	void	VRenderButton(const char *text, int x, int y, int r, int g, int b) override;
	void	VRenderAllButtons() override;
	void	VCreateButtonImage(_renderButtonData *data) override;
	void	VCleanButtonImages() override;
	bool	VCleanButtonImage(int id) override;

	void	VCleanAllSelectBoxImages() override;
	bool	VCleanSelectBoxImage(int id) override;
	void	VRenderAllSelectBox() override;
	void	VCreateSelectBoxImage(int id, const std::vector<std::string> &text, int x, int y, int startY, int sizeY, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;

	void	VCreateTextInputImage(int id, char *title, const std::string &text, int x, int y, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;
	void	VCleanAllTextInputImages() override;
	bool	VCleanTextInputImage(int id) override;
	void	VRenderAllTextInputs() override;

	void	VRenderHUD() override;
	void	VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, bool callDisplay = true) override;
	void	VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, int imgIndex, int imgType) override;
	void	VCreateInGameHUDImage(int frameRate, int sec, int mouseX, int mouseY, int hp, int mp) override;
	void	VCreateInGameHoverData(const _inGameHoverData *hoverData) override;
	void	VCreateInGameHUDData(RenderHudType hudType, const std::vector<int> &imgIndex, int x, int y, int w, int h, int offsetX) override;

	void	VCreateGameMap(const _mapTileAll *map) override;
	void	VRenderGameMap(const int cameraX, const int cameraY) override;

	bool	VValidId(int id_, const ImgTextureType &type)override;

	sf::RenderWindow*	VGetRenderWindow() override;
};
