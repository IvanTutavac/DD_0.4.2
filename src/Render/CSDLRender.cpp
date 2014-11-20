#include "CSDLRender.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../Logic/Entity/EntityData.h"
#include "../Data/Const.h"
#include "../Data/Globals.h"
#include "../Data/Enums.h"
#include "../Logic/Map/MapData.h"
#include "../Tools/Log.h"
#include "../Tools/StringWrapper.h"
#include "../Tools/debug.h"
#include "../IO/IOFunctions.h"


CSDLRender::CSDLRender()
{
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
	m_pFont = nullptr;
	m_pFont10 = nullptr;
	m_pFont12 = nullptr;
	m_pFont14 = nullptr;
	m_pFont16 = nullptr;
	m_pHUDImg = nullptr;
	m_pButtonRect = nullptr;
	m_pMapTexture = nullptr;
	m_hoverTexture.texture = nullptr;
}

CSDLRender::~CSDLRender()
{

}

bool	CSDLRender::VInit()
{
	SDL_Init(SDL_INIT_VIDEO);

	m_pWindow = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_windowWidth, g_windowHeight + g_windowHUDHeight, SDL_WindowFlags::SDL_WINDOW_SHOWN);

	if (m_pWindow == nullptr)
	{
		Log("Window creation failed");
		return	false;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	if (m_pRenderer == nullptr)
	{
		Log("Renderer creation failed");
		return	false;
	}

	if (TTF_Init() == -1)
	{
		Log("TTF_Init failed");
		return	false;
	}

	if (LoadImages() == false)
	{
		Log("LoadImages failed");
		return	false;
	}

	if (LoadFonts() == false)
	{
		Log("LoadFonts failed");
		return	false;
	}

	// just for testing
	//SDL_SetWindowGrab(m_pWindow, SDL_TRUE);

	m_hoverTextureReady = false;

	return	true;
}

void	CSDLRender::VClean()
{
	for (size_t i = 0; i < m_vImage.size(); i++)
	{
		m_vImage[i].Clean();
	}

	m_vImage.clear();

	for (HudDataSDLIterator it = m_hudData.begin(); it != m_hudData.end(); ++it)
	{
		SDL_DestroyTexture(it->second.texture);
	}

	m_hudData.clear();

	VCleanButtonImages();
	VCleanAllTextInputImages();
	VCleanAllSelectBoxImages();

	SDL_DestroyTexture(m_pButtonRect);
	SDL_DestroyTexture(m_pHUDImg);
	SDL_DestroyTexture(m_pMapTexture);
	SDL_DestroyTexture(m_hoverTexture.texture);

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	TTF_CloseFont(m_pFont);
	TTF_CloseFont(m_pFont10);
	TTF_CloseFont(m_pFont12);
	TTF_CloseFont(m_pFont14);
	TTF_CloseFont(m_pFont16);

	TTF_Quit();

	SDL_Quit();
}

void	CSDLRender::VClearWindow()
{
	SDL_RenderClear(m_pRenderer);
}

void	CSDLRender::VDrawWindow()
{
	SDL_RenderPresent(m_pRenderer);
}

void	CSDLRender::VRenderLine(const char * const text, int x, const int y, const ColorIndex &olorIndex, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{

}

void	CSDLRender::VRenderLine(const char * const text, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	SDL_Color fontColor;
	fontColor.r = r, fontColor.g = g, fontColor.b = b;

	RenderLine(text, x, y, GetFont(fontSize, fontType), &fontColor, flag, x1, x2);
}

void	CSDLRender::VRenderValue(int value, int x, const int y, const ColorIndex &olorIndex, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{

}

void	CSDLRender::VRenderValue(int value, int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	SDL_Color fontColor;
	fontColor.r = r, fontColor.g = g, fontColor.b = b;

	char text[25];

	DD_Itoa(value, text, sizeof(text), 10);

	SDL_Surface *surface = nullptr;

	surface = TTF_RenderText_Solid(GetFont(fontSize, fontType), text, fontColor);

	if ((flag & TextFlag::nothing) == 0)
		AlignmentX(flag, x, x1, surface->w, x2);

	RenderImage(x, y, surface->w, surface->h, surface);

	SDL_FreeSurface(surface);
	surface = nullptr;
}

void	CSDLRender::VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType)
{
	RenderImage(x, y, w, h, m_vImage[imgType].m_vTexture[imgIndex].texture);
}

void	CSDLRender::VRenderImage(const int x, const int y, const int imgIndex, const int imgType)
{
	RenderImage(x, y, m_vImage[imgType].m_vTexture[imgIndex].w, m_vImage[imgType].m_vTexture[imgIndex].h, m_vImage[imgType].m_vTexture[imgIndex].texture);
}

void	CSDLRender::VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType, double angle)
{
	RenderImageEx(x, y, w, h, angle, m_vImage[imgType].m_vTexture[imgIndex].texture);
}

void	CSDLRender::VRenderButton(const char *text, int x, int y, int r, int g, int b)
{
	SDL_Color	fontColor;
	fontColor.r = r, fontColor.g = g, fontColor.b = b;

	SDL_Surface *surface{ nullptr };
	SDL_Texture *texture{ nullptr };

	surface = TTF_RenderText_Solid(m_pFont, text, fontColor); // render text

	texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);

	int h{ m_vImage[TYPE_IMG_GUI].m_vTexture[IMG_INDEX_BUTTON].h };
	int w{ m_vImage[TYPE_IMG_GUI].m_vTexture[IMG_INDEX_BUTTON].w };

	int k{ (x + w / 2 - surface->w / 2) };
	int l{ (y + h / 2 - surface->h / 2) };

	// render button img
	RenderImage(x, y, w, h, m_vImage[TYPE_IMG_GUI].m_vTexture[IMG_INDEX_BUTTON].texture);

	// render button with text
	RenderImage(k, l, surface->w, surface->h, texture);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	surface = nullptr, texture = nullptr;
}

void	CSDLRender::VRenderAllButtons()
{
	for (size_t i = 0; i < m_vButtonImage.size(); i++)
	{
		RenderImage(m_vButtonImage[i].x, m_vButtonImage[i].y, m_vButtonImage[i].w, m_vButtonImage[i].h, m_vButtonImage[i].texture);
	}
}

void	CSDLRender::VCreateButtonImage(_renderButtonData *data)
{
	_imgTexture img;
	img.x = data->x, img.y = data->y, img.w = data->w, img.h = data->h;
	img.r = data->r2, img.g = data->g2, img.b = data->b2;
	img.id = data->id;

	SDL_Color fontColor;
	fontColor.r = img.r, fontColor.g = img.g, fontColor.b = img.b;

	img.texture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, img.w, img.h);

	SDL_SetRenderTarget(m_pRenderer, img.texture);

	RenderImage(0, 0, img.w, img.h, m_pButtonRect);

	SDL_Surface *textSurface = TTF_RenderText_Solid(m_pFont, data->text, fontColor);

	// used for text centering
	int k = img.w / 2 - textSurface->w / 2;
	int l = img.h / 2 - textSurface->h / 2;

	RenderImage(k, l, textSurface->w, textSurface->h, textSurface);

	m_vButtonImage.push_back(img);

	SDL_SetRenderTarget(m_pRenderer, nullptr);

	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
}

void	CSDLRender::VCleanButtonImages()
{
	for (size_t i = 0; i < m_vButtonImage.size(); i++)
	{
		SDL_DestroyTexture(m_vButtonImage[i].texture);
		m_vButtonImage[i].texture = nullptr;
	}

	m_vButtonImage.clear();
}

bool	CSDLRender::VCleanButtonImage(int id)
{
	if (m_vButtonImage.size() < 1)
	{
		Log("CSDLRender::VCleanButtonImage(int) failed, size < 1");
		return	false;
	}

	for (int i = m_vButtonImage.size() - 1; i >= 0; i--)
	{
		if (m_vButtonImage[i].id == id)
		{
			SDL_DestroyTexture(m_vButtonImage[i].texture);
			m_vButtonImage.erase(m_vButtonImage.begin() + i);
			return	true;
		}
	}

	Log("CSDLRender::VCleanButtonImage(int) failed, id not found, id = ", id);

	return	false;
}

bool	CSDLRender::VCleanSelectBoxImage(int id)
{
	if (m_vSelectBoxImage.size() < 1)
	{
		Log("CSDLRender::VCleanSelectBoxImage failed, there's no select box images");
		return	false;
	}

	for (int i = m_vSelectBoxImage.size() - 1; i >= 0; i--)
	{
		if (m_vSelectBoxImage[i].id == id)
		{
			SDL_DestroyTexture(m_vSelectBoxImage[i].texture);
			m_vSelectBoxImage.erase(m_vSelectBoxImage.begin() + i);
			return	true;
		}
	}

	Log("CSDLRender::VCleanSelectBoxImage failed, there's no selectBox with id: ", id);

	return	false;
}

void	CSDLRender::VCleanAllSelectBoxImages()
{
	for (size_t i = 0; i < m_vSelectBoxImage.size(); i++)
	{
		SDL_DestroyTexture(m_vSelectBoxImage[i].texture);
		m_vSelectBoxImage[i].texture = nullptr;
	}

	m_vSelectBoxImage.clear();
}

void	CSDLRender::VCleanAllTextInputImages()
{
	for (size_t i = 0; i < m_vTextInputImage.size(); i++)
	{
		SDL_DestroyTexture(m_vTextInputImage[i].texture);
		m_vTextInputImage[i].texture = nullptr;
	}

	m_vTextInputImage.clear();
}

bool	CSDLRender::VCleanTextInputImage(int id)
{
	if (m_vTextInputImage.size() < 1)
	{
		Log("VCleanTextInputImage failed, size < 1");
		return	false;
	}

	for (int i = m_vTextInputImage.size() - 1; i >= 0; i--)
	{
		if (m_vTextInputImage[i].id == id)
		{
			SDL_DestroyTexture(m_vTextInputImage[i].texture);
			m_vTextInputImage.erase(m_vTextInputImage.begin() + i);
			return	true;
		}
	}

	Log("VCleanTextInputImage failed, id not found, id = ", id);

	return	false;
}

void	CSDLRender::VRenderHUD()
{
	RenderImage(0, g_windowHeight, g_windowWidth, g_windowHUDHeight, m_pHUDImg);

	if (m_hoverTextureReady)
		RenderImage(m_hoverTexture.x, m_hoverTexture.y, m_hoverTexture.w, m_hoverTexture.h, m_hoverTexture.texture);
}

void	CSDLRender::VRenderAllSelectBox()
{
	for (size_t i = 0; i < m_vSelectBoxImage.size(); i++)
	{
		RenderImage(m_vSelectBoxImage[i].x, m_vSelectBoxImage[i].y, m_vSelectBoxImage[i].w, m_vSelectBoxImage[i].h, m_vSelectBoxImage[i].texture);
	}
}

void	CSDLRender::VRenderAllTextInputs()
{
	for (size_t i = 0; i < m_vTextInputImage.size(); i++)

	{
		RenderImage(m_vTextInputImage[i].x, m_vTextInputImage[i].y, m_vTextInputImage[i].w, m_vTextInputImage[i].h, m_vTextInputImage[i].texture);
	}
}

void	CSDLRender::VCreateSelectBoxImage(int id, const std::vector<std::string> &text, int x, int y, int startY, int sizeY, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	if (text.size() < 1)
		return;

	_imgTexture texture;

	texture.x = x;
	texture.y = y;
	texture.w = w;
	texture.h = h;
	texture.id = id;

	texture.texture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h); // empty select box texture

	SDL_SetTextureBlendMode(m_pHUDImg, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(m_pRenderer, texture.texture);

	ClearRenderTarget(r1, g1, b1, 255);

	SDL_Color c;
	c.r = r2, c.g = g2, c.b = b2;

	int currentY = startY;

	for (size_t i = 0; i < text.size(); i++)
	{
		RenderLine(text[i].c_str(), 0, currentY, GetFont(fontSize, fontType), &c, flag, x1, x2);
		currentY += sizeY;
	}

	SDL_SetRenderTarget(m_pRenderer, nullptr);

	m_vSelectBoxImage.push_back(texture);
}

void	CSDLRender::VCreateTextInputImage(int id, char *title, const std::string &text, int x, int y, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	_imgTexture texture;

	texture.x = x;
	texture.y = y;
	texture.w = w;
	texture.h = h;
	texture.id = id;

	texture.texture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h); // empty text box texture

	SDL_SetTextureBlendMode(texture.texture, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(m_pRenderer, texture.texture);

	ClearRenderTarget(r1, g1, b1, 255);

	SDL_Color c;
	c.r = r2, c.g = g2, c.b = b2;

	if (title != nullptr)
		RenderLine(title, 8, 20, GetFont(fontSize, fontType), &c); // x and y are coordinates on the texture that acts as a renderer destination

	if (text.length() > 0)
		RenderLine(text.c_str(), 8, h / 2, GetFont(fontSize, fontType), &c);

	SDL_SetRenderTarget(m_pRenderer, nullptr);

	m_vTextInputImage.push_back(texture);
	// SDL_DestroyTexture(texture.texture); // doing  this is bad because it will also delete it from m_vTextInputImage
}

void	CSDLRender::VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, bool callDisplay)
{
	SDL_SetRenderTarget(m_pRenderer, m_pHUDImg);

	ClearRenderTarget(168, 237, 198, 255);

	VRenderValue(frameRate, 0, 8, 100, 0, 100, FontSize::font16, FontType::LiberationMono_Regular, TextFlag::alignRight, g_windowWidth);
	VRenderValue(sec, 0, 23, 100, 0, 100, FontSize::font16, FontType::LiberationMono_Regular, TextFlag::alignRight, g_windowWidth);
	VRenderValue(mouseX, g_windowWidth - 120, 40, 100, 0, 100, FontSize::font16, FontType::LiberationMono_Regular);
	VRenderValue(mouseY, g_windowWidth - 60, 40, 100, 0, 100, FontSize::font16, FontType::LiberationMono_Regular);

	if (callDisplay)
	{
		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}
}

void	CSDLRender::VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, int imgIndex, int imgType)
{
	VCreateHUDImage(frameRate, sec, mouseX, mouseY, false);

	if ((int)m_vImage[imgType].m_vTexture.size() <= imgIndex) // does the image exist ?
	{
		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}

	SDL_Rect rect;
	rect.x = g_windowWidth / 2 + 130, rect.y = 8, rect.w = TILE_SIZE, rect.h = TILE_SIZE;

	SDL_Color color;
	color.r = 0, color.g = 0, color.b = 0;

	// x and y not in the window position, but in the texture positions
	RenderLine("Selected tile: ", 0, 10, m_pFont16, &color, TextFlag::alignRight, g_windowWidth / 2 + 130);

	SDL_RenderCopy(m_pRenderer, m_vImage[imgType].m_vTexture[imgIndex].texture, nullptr, &rect); // rendering current selected tile

	SDL_SetRenderTarget(m_pRenderer, nullptr);
}

void	CSDLRender::VCreateInGameHUDImage(int frameRate, int sec, int mouseX, int mouseY, int hp, int mp)
{
	VCreateHUDImage(frameRate, sec, mouseX, mouseY, false);

	// rendering target is the hud texture, so it's not the window 

	SDL_Color color{};
	color.r = 0, color.g = 0, color.b = 255;

	RenderLine(std::string{ "HP: " + std::to_string(hp) }.c_str(), 8, 8, GetFont(FontSize::font14, FontType::LiberationMono_Regular), &color);
	RenderLine(std::string{ "MP: " + std::to_string(mp) }.c_str(), 8, 36, GetFont(FontSize::font14, FontType::LiberationMono_Regular), &color);

	int k{ 0 };
	for (HudDataSDLIterator it = m_hudData.begin(); it != m_hudData.end(); ++it, ++k)
	{
		RenderImage(it->second.x, it->second.y, it->second.w, it->second.h, it->second.texture);
	}

	SDL_SetRenderTarget(m_pRenderer, nullptr);
}

void	CSDLRender::VCreateInGameHoverData(const _inGameHoverData *hoverData)
{
	if (hoverData->data.size() < 1)
	{
		m_hoverTextureReady = false;
		return;
	}

	if (m_hoverTexture.texture != nullptr)
		SDL_DestroyTexture(m_hoverTexture.texture);

	m_hoverTexture.x = hoverData->posX, m_hoverTexture.y = hoverData->posY;
	m_hoverTexture.w = hoverData->w, m_hoverTexture.h = hoverData->h;

	m_hoverTexture.texture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_hoverTexture.w, m_hoverTexture.h);

	SDL_SetTextureBlendMode(m_hoverTexture.texture, SDL_BLENDMODE_BLEND);

	SDL_SetRenderTarget(m_pRenderer, m_hoverTexture.texture);

	ClearRenderTarget(168, 237, 198, 255);

	SDL_Color color{ 45, 55, 155 };

	if (hoverData->inGameHoverType == InGameHoverType::enemy || hoverData->inGameHoverType == InGameHoverType::hudSpell || hoverData->inGameHoverType == InGameHoverType::hudWeapAtt)
	{
		int y = 0;
		for (const auto& data : hoverData->data)
		{
			RenderLine(data.c_str(), 1, y, GetFont(FontSize::font12, FontType::LiberationMono_Regular), &color, TextFlag::center, 0, m_hoverTexture.w);
			y += 12;
		}
	}

	SDL_SetRenderTarget(m_pRenderer, nullptr);

	m_hoverTextureReady = true;
}

void	CSDLRender::VCreateInGameHUDData(RenderHudType hudType, const std::vector<int> &imgIndex, int x, int y, int w, int h, int offsetX)
{
	HudDataSDLIterator it{};

	if ((it = m_hudData.find(hudType)) != m_hudData.end())
	{
		SDL_DestroyTexture(it->second.texture);
	}
	
	_imgTexture	&hud{ m_hudData[hudType] };

	int width{ static_cast<int>(imgIndex.size() * offsetX) };

	hud.texture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, h);
	hud.x = x;
	hud.y = y;
	hud.w = width;
	hud.h = h;

	SDL_SetRenderTarget(m_pRenderer, hud.texture);

	ClearRenderTarget(168, 237, 198, 255);

	int posX{ 0 };

	int	imgType{}, onHudY{ 0 };

	if (hudType == RenderHudType::spells)
		imgType = TYPE_IMG_SPELL, onHudY = 0;
	else if (hudType == RenderHudType::weaponAttacks)
		imgType = TYPE_IMG_ITEM, onHudY = 32;

	for (size_t i = 0; i < imgIndex.size(); i++)
	{
		RenderImage(posX, y, w, h, m_vImage[imgType].m_vTexture[imgIndex[i]].texture); 
		posX += offsetX;
	}

	SDL_SetRenderTarget(m_pRenderer, nullptr);

	hud.y = onHudY; // old y was position on hud.texture (texture for hud data on spells/weap...), new position is for the whole hud texture
}

void	CSDLRender::VCreateGameMap(const _mapTileAll *map)
{
	SDL_DestroyTexture(m_pMapTexture);
	m_pMapTexture = nullptr;

	if (map->currentMap == MapType::small_)
	{
		m_pMapTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, MAP_WIDTH_SMALL, MAP_HEIGHT_SMALL);

		SDL_SetRenderTarget(m_pRenderer, m_pMapTexture);

		for (int i = 0; i < MAP_HEIGHT_SMALL / 32; i++)
		{
			for (int j = 0; j < MAP_WIDTH_SMALL / 32; j++)
			{
				VRenderImage(j * 32, i * 32, 32, 32, map->small_->map[i][j].imgIndex, TYPE_IMG_FLOOR);
			}
		}

		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}
	else if (map->currentMap == MapType::medium)
	{
		m_pMapTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, MAP_WIDTH_MEDIUM, MAP_HEIGHT_MEDIUM);

		SDL_SetRenderTarget(m_pRenderer, m_pMapTexture);

		for (int i = 0; i < MAP_HEIGHT_MEDIUM / 32; i++)
		{
			for (int j = 0; j < MAP_WIDTH_MEDIUM / 32; j++)
			{
				VRenderImage(j * 32, i * 32, map->medium->map[i][j].imgIndex, TYPE_IMG_FLOOR);
			}
		}

		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}
	else if (map->currentMap == MapType::big)
	{
		m_pMapTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, MAP_WIDTH_BIG, MAP_HEIGHT_BIG);

		SDL_SetRenderTarget(m_pRenderer, m_pMapTexture);

		for (int i = 0; i < MAP_HEIGHT_BIG / 32; i++)
		{
			for (int j = 0; j < MAP_WIDTH_BIG / 32; j++)
			{
				VRenderImage(j * 32, i * 32, map->big->map[i][j].imgIndex, TYPE_IMG_FLOOR);
			}
		}

		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}
}

void	CSDLRender::VRenderGameMap(const int cameraX, const int cameraY)
{
	int startX{ cameraX - g_windowWidth / 2 }, startY{ cameraY - g_windowHeight / 2 };

	SDL_Rect	srcRect;
	srcRect.x = startX, srcRect.y = startY, srcRect.w = g_windowWidth, srcRect.h = g_windowHeight;

	SDL_Rect	destRect;
	destRect.x = 0, destRect.y = 0, destRect.w = g_windowWidth, destRect.h = g_windowHeight;

	RenderImage(&srcRect, &destRect, m_pMapTexture);
}

bool	CSDLRender::VValidId(int id_, const ImgTextureType &type)
{
	if (type == ImgTextureType::textBoxInput)
	{
		for (size_t i = 0; i < m_vTextInputImage.size(); i++)
		{
			if (m_vTextInputImage[i].id == id_)
			{
				return	false;
			}
		}
	}
	else if (type == ImgTextureType::button)
	{
		for (size_t i = 0; i < m_vButtonImage.size(); i++)
		{
			if (m_vButtonImage[i].id == id_)
			{
				return	false;
			}
		}
	}

	return	true;
}

// private

bool	CSDLRender::LoadImages()
{
	CSDLImage	floorImg{};
	CSDLImage	guiImg{};
	CSDLImage	monstersImg{};
	CSDLImage	spellsImg{};
	CSDLImage	playerImg{};
	CSDLImage	itemImg{};

	if (floorImg.Init(m_pRenderer, IMG_FLOOR_START, IMG_FLOOR_FINISH, "floor/") == false)
	{
		floorImg.Clean();
		Log("CSDLRender::LoadImages() failed, Floor img load failed");
		return	false;
	}

	if (guiImg.Init(m_pRenderer, IMG_GUI_START, IMG_GUI_FINISH, "gui/") == false)
	{
		guiImg.Clean();
		Log("CSDLRender::LoadImages() failed, GUI img load failed");
		return	false;
	}

	if (monstersImg.Init(m_pRenderer, IMG_MONSTER_START, IMG_MONSTER_FINISH, "monster/") == false)
	{
		monstersImg.Clean();
		Log("CSDLRender::LoadImages() failed, Monster img load failed");
		return	false;
	}

	if (spellsImg.Init(m_pRenderer, IMG_SPELL_START, IMG_SPELL_FINISH, "spell/") == false)
	{
		spellsImg.Clean();
		Log("CSDLRender::LoadImages() failed, Spell img load failed");
		return	false;
	}

	if (playerImg.Init(m_pRenderer, IMG_PLAYER_START, IMG_PLAYER_FINISH, "player/") == false)
	{
		playerImg.Clean();
		Log("CSDLRender::LoadImages() failed, Player img load failed");
		return	false;
	}

	if (itemImg.Init(m_pRenderer, IMG_ITEM_START, IMG_ITEM_FINISH, "item/") == false)
	{
		itemImg.Clean();
		Log("CSDLRender::LoadImages() failed, Item img load failed");
		return	false;
	}

	// order matters, decided by IMG_TYPE_ in Const.h

	m_vImage.push_back(floorImg);
	m_vImage.push_back(guiImg);
	m_vImage.push_back(monstersImg);
	m_vImage.push_back(spellsImg);
	m_vImage.push_back(playerImg);
	m_vImage.push_back(itemImg);

	SDL_Surface *tmpButtonRect = LoadSurface("img/gui/1.png");

	if (tmpButtonRect == nullptr)
	{
		Log("CSDLRender::LoadImages() failed, 1.png button surface failed to load");
		return	false;
	}

	m_pButtonRect = SDL_CreateTextureFromSurface(m_pRenderer, tmpButtonRect);

	bool load = DD::IO::LoadPassableFloor(&m_vImage[0].m_vTexture, nullptr);

	if (load == false)
	{
		Log("CSDLRender::LoadImages() failed, floor img passable setup failed");
		return	false;
	}

	m_pHUDImg = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, g_windowWidth, g_windowHUDHeight);

	SDL_SetTextureBlendMode(m_pHUDImg, SDL_BLENDMODE_BLEND);

	return	true;
}

bool	CSDLRender::LoadFonts()
{
	m_pFont = LoadFont("LiberationMono-Regular.ttf", 22);

	if (m_pFont == nullptr)
		return	false;

	m_pFont16 = LoadFont("LiberationMono-Regular.ttf", 16);

	if (m_pFont16 == nullptr)
		return	false;

	m_pFont14 = LoadFont("LiberationMono-Regular.ttf", 14);

	if (m_pFont14 == nullptr)
		return	false;

	m_pFont12 = LoadFont("LiberationMono-Regular.ttf", 12);

	if (m_pFont12 == nullptr)
		return	false;

	m_pFont10 = LoadFont("LiberationMono-Regular.ttf", 10);

	if (m_pFont10 == nullptr)
		return false;

	return	true;
}

TTF_Font* CSDLRender::LoadFont(const char * fontName, const int fontSize)
{
	TTF_Font* font = TTF_OpenFont(fontName, fontSize);

	if (font == nullptr)
	{
		char msg[50];
		DD_Strcat(msg, sizeof(msg), fontName);
		DD_Strcat(msg, sizeof(msg), " failed to load");

		Log(msg);

		return	nullptr;
	}

	return	font;
}

void	CSDLRender::RenderLine(const char *text, int x, const int y, TTF_Font *font, SDL_Color *color, TextFlag::TextFlag flag, int x1, int x2)
{
	SDL_Surface *surface = nullptr;

	surface = TTF_RenderText_Solid(font, text, *color);

	if ((flag & TextFlag::nothing) == 0) // if flag is not set
		AlignmentX(flag, x, x1, surface->w, x2);

	RenderImage(x, y, surface->w, surface->h, surface);

	SDL_FreeSurface(surface);
	surface = nullptr;
}

void	CSDLRender::RenderImage(const int x, const int y, const int w, const int h, SDL_Surface *surface)
{
	SDL_Texture *texture = nullptr;
	SDL_Rect tRect;

	texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);

	tRect.x = x, tRect.y = y, tRect.w = w, tRect.h = h;

	SDL_RenderCopy(m_pRenderer, texture, nullptr, &tRect);
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void	CSDLRender::RenderImage(const int x, const int y, const int w, const int h, SDL_Texture *texture)
{
	SDL_Rect	rect;
	rect.x = x, rect.y = y, rect.w = w, rect.h = h;

	SDL_RenderCopy(m_pRenderer, texture, nullptr, &rect);
}

void	CSDLRender::RenderImage(const SDL_Rect *srcRect, const SDL_Rect *desRect, SDL_Texture *texture)
{
	SDL_RenderCopy(m_pRenderer, texture, srcRect, desRect);
}

void	CSDLRender::RenderImageEx(const int x, const int y, const int w, const int h, const double angle, SDL_Texture *texture)
{
	SDL_Rect	rect;
	rect.x = x, rect.y = y, rect.w = w, rect.h = h;

	SDL_RenderCopyEx(m_pRenderer, texture, nullptr, &rect, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

TTF_Font*	CSDLRender::GetFont(const FontSize fontSize, const FontType fontType)
{
	if (fontType == FontType::LiberationMono_Regular)
	{
		if (fontSize == FontSize::font10)
		{
			return	m_pFont10;
		}
		if (fontSize == FontSize::font12)
		{
			return	m_pFont12;
		}
		else if (fontSize == FontSize::font14)
		{
			return	m_pFont14;
		}
		else if (fontSize == FontSize::font16)
		{
			return	m_pFont16;
		}
		else if (fontSize == FontSize::font22)
		{
			return	m_pFont;
		}
		else
		{
			Log("Wrong font size used");
			return	nullptr;
		}
	}
	else
	{
		Log("Wrong font used");
		return	nullptr;
	}

	return	nullptr;
}

void	CSDLRender::ClearRenderTarget(unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, unsigned __int8 a)
{
	Uint8	currentR, currentG, currentB, currentA;

	SDL_GetRenderDrawColor(m_pRenderer, &currentR, &currentG, &currentB, &currentA);

	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);

	SDL_RenderFillRect(m_pRenderer, nullptr);

	SDL_SetRenderDrawColor(m_pRenderer, currentR, currentG, currentB, currentA);
}
