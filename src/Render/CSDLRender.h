#pragma once

#include <map>

#include "CRender.h"
#include "CSDLImage.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;
typedef struct _TTF_Font TTF_Font;

enum class ColorIndex;

struct _imgTexture
{
	int x, y, w, h;
	int r, g, b;
	int	id; // id of the image, used if one wants to delete a specific image
	SDL_Texture *texture;
};

typedef std::map<RenderHudType, _imgTexture>::iterator HudDataSDLIterator;

class CSDLRender : public CRender
{
private:

	std::vector<CSDLImage>	m_vImage; // m_vImage[TYPE_IMAGE].m_vTexture[IMG_INDEX].texture

	SDL_Window	*m_pWindow;
	SDL_Renderer *m_pRenderer;
	TTF_Font	*m_pFont; // size 22
	TTF_Font	*m_pFont10;
	TTF_Font	*m_pFont12;
	TTF_Font	*m_pFont14;
	TTF_Font	*m_pFont16;

	SDL_Texture *m_pButtonRect;
	SDL_Texture *m_pHUDImg;
	SDL_Texture	*m_pMapTexture;

	std::vector<_imgTexture>	m_vButtonImage;
	std::vector<_imgTexture>	m_vTextInputImage;
	std::vector<_imgTexture>	m_vSelectBoxImage;

	bool		m_hoverTextureReady;
	_imgTexture	m_hoverTexture;

	std::map<RenderHudType, _imgTexture>	m_hudData;

private:

	// loads all .png img files into m_vImage 
	bool	LoadImages();

	// checks for nullptr returns false if one was found
	bool	LoadFonts();

	// does error checking and writes to log
	TTF_Font*	LoadFont(const char* fontName,const int fontSize);

	// creates a texture and renders it
	void	RenderImage(const int x, const int y, const int w, const int h, SDL_Surface *surface);
	void	RenderImage(const int x, const int y, const int w, const int h, SDL_Texture *texture);
	// pass nullptr for srcRect if you want the whole texture to be rendered
	// pass nullptr for desRect if  you want the texture rendered over the whole rendering target
	void	RenderImage(const SDL_Rect *srcRect, const SDL_Rect *desRect, SDL_Texture *texture);
	void	RenderImageEx(const int x, const int y, const int w, const int h, const double angle, SDL_Texture *texture);

	void	RenderLine(const char *text, int x, const int y, TTF_Font *font, SDL_Color *color, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1);

	// get the font fontType with the size fontSize
	TTF_Font*	GetFont(const FontSize fontSize,const FontType fontType);

	// Clears the render target with a color
	void	ClearRenderTarget(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a);

public:

	CSDLRender();
	~CSDLRender();

	bool	VInit() override;
	void	VClean() override;

public:

	void	VClearWindow() override;
	void	VDrawWindow() override;

	void	VRenderLine(const char* const text, int x, const int y, const ColorIndex  &colorIndex, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;
	// uses default font (m_pFont)
	void	VRenderLine(const char* const text, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;

	void	VRenderValue(const int value, int x, const int y, const ColorIndex  &colorIndex, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;
	// uses default font (m_pFont)
	void	VRenderValue(const int value, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType = FontType::LiberationMono_Regular, TextFlag::TextFlag flag = TextFlag::nothing, int x1 = -1, int x2 = -1) override;

	void	VRenderImage(const int x, const int y, const int w,const int h, const int imgIndex,const int imgType) override;
	void	VRenderImage(const int x, const int y,  const int imgIndex, const int imgType) override;
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

	bool	VValidId(int id_, const ImgTextureType &type) override;
};
