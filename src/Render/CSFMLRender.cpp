#include "CSFMLRender.h"
#include "../Logic/Map/MapData.h"
#include "../Logic/Entity/EntityData.h"
#include "../Data/Globals.h"
#include "../Data/Enums.h"
#include "../Data/Const.h"
#include "../Tools/debug.h"
#include "../Tools/StringWrapper.h"
#include "../Tools/Log.h"
#include "../IO/IOFunctions.h"
#include <SFML/Graphics.hpp>

CSFMLRender::CSFMLRender()
{
	m_pWindow = nullptr;
	m_pFont = nullptr;
	m_pHUDTexture = nullptr;
	m_pHUDSprite = nullptr;
	m_pMapTexture = nullptr;
	m_pMapSprite = nullptr;
	m_pHoverTexture = nullptr;
	m_pHoverSprite = nullptr;
}

CSFMLRender::~CSFMLRender()
{

}

bool	CSFMLRender::VInit()
{
	m_pWindow = DD_NEW sf::RenderWindow(sf::VideoMode(g_windowWidth, g_windowHeight + g_windowHUDHeight), APP_NAME, sf::Style::Close);

	if (!LoadImages())
	{
		Log("CSFMLRender::VInit() failed, LoadImages() call returned false");
		return	false;
	}

	if (!LoadFonts())
	{
		Log("CSFMLRender::VInit() failed, LoadFonts() call returned false");
	}

	m_hoverTextureReady = false;

	return	true;
}

void	CSFMLRender::VClean()
{
	for (size_t i = 0; i < m_vImage.size(); i++)
	{
		m_vImage[i].Clean();
	}

	m_vImage.clear();

	for (HudDataSFMLIterator it = m_hudData.begin(); it != m_hudData.end(); ++it)
	{
		DD_DELETE(it->second.sprite);
		DD_DELETE(it->second.texture);
	}

	m_hudData.clear();

	VCleanButtonImages();
	VCleanAllSelectBoxImages();
	VCleanAllTextInputImages();

	DD_DELETE(m_pHUDSprite);
	DD_DELETE(m_pHUDTexture);

	DD_DELETE(m_pMapSprite);
	DD_DELETE(m_pMapTexture);

	DD_DELETE(m_pHoverSprite);
	DD_DELETE(m_pHoverTexture);

	m_pWindow->close();

	DD_DELETE(m_pWindow);
	DD_DELETE(m_pFont);
}

void	CSFMLRender::VClearWindow()
{
	m_pWindow->clear();
}

void	CSFMLRender::VDrawWindow()
{
	m_pWindow->display();
}

void	CSFMLRender::VRenderLine(const char* const text, const int x, const int y, const ColorIndex  &colorIndex, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{

}

void	CSFMLRender::VRenderLine(const char* const text, const int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	RenderLine(nullptr, text, x, y, GetFont(fontType), GetFontSize(fontSize), r, g, b, flag, x1, x2);
}

void	CSFMLRender::VRenderValue(const int value, const int x, const int y, const ColorIndex  &colorIndex, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{

}

void	CSFMLRender::VRenderValue(const int value, const int x, const int y, const int r, const int g, const int b, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	sf::Color color;
	color.r = r;
	color.g = g;
	color.b = b;

	sf::Text text(std::to_string(value), *GetFont(fontType), GetFontSize(fontSize));

	text.setColor(color);
	text.setPosition(static_cast<float>(x), static_cast<float>(y));

	m_pWindow->draw(text);
}

void	CSFMLRender::VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType)
{
	RenderImage(x, y, m_vImage[imgType].m_vTexture[imgIndex].sprite);
}

void	CSFMLRender::VRenderImage(const int x, const int y, const int imgIndex, const int imgType)
{
	RenderImage(x, y, m_vImage[imgType].m_vTexture[imgIndex].sprite);
}

void	CSFMLRender::VRenderImage(const int x, const int y, const int w, const int h, const int imgIndex, const int imgType, double angle)
{
	RenderImageEx(x, y, w, h, angle, 16, 16, m_vImage[imgType].m_vTexture[imgIndex].sprite);
}

void	CSFMLRender::VRenderButton(const char *text, int x, int y, int r, int g, int b)
{
	sf::RenderTexture	BtnTexture{};

	const	int	btnW = 192;
	const	int	btnH = 64;

	BtnTexture.create(btnW, btnH);
	BtnTexture.clear(sf::Color{ 255, 255, 255 });

	sf::Text	Text{ text, *m_pFont, 22 };

	Text.setColor(sf::Color{ static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b) });

	sf::FloatRect	rect = Text.getLocalBounds();

	float x_{ (btnW / 2) - (rect.width / 2) };
	float y_{ (btnH / 2) - (rect.height / 2) };

	Text.setPosition(sf::Vector2f{ x_, y_ });

	BtnTexture.draw(Text);
	BtnTexture.display();

	sf::Sprite	sprite{ BtnTexture.getTexture() };

	sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

	m_pWindow->draw(sprite);
}

sf::RenderWindow*	CSFMLRender::VGetRenderWindow()
{
	return	m_pWindow;
}

void	CSFMLRender::VRenderAllButtons()
{
	for (size_t i = 0; i < m_vButtonImage.size(); i++)
	{
		RenderImage(m_vButtonImage[i].x, m_vButtonImage[i].y, m_vButtonImage[i].sprite);
	}
}

void	CSFMLRender::VRenderAllTextInputs()
{
	for (size_t i = 0; i < m_vTextInputImage.size(); i++)
	{
		RenderImage(m_vTextInputImage[i].x, m_vTextInputImage[i].y, m_vTextInputImage[i].sprite);
	}
}

void	CSFMLRender::VCreateButtonImage(_renderButtonData *data)
{
	_imgTextureSFML img;
	img.x = data->x, img.y = data->y, img.w = data->w, img.h = data->h;
	img.r = data->r2, img.g = data->g2, img.b = data->b2;
	img.id = data->id;

	sf::RenderTexture	BtnTexture{};

	BtnTexture.create(img.w, img.h - 2);
	BtnTexture.clear(sf::Color{ static_cast<sf::Uint8>(data->r1), static_cast<sf::Uint8>(data->g1), static_cast<sf::Uint8>(data->b1) });

	sf::Text	Text{ data->text, *m_pFont, 22 };

	Text.setColor(sf::Color{ static_cast<sf::Uint8>(img.r), static_cast<sf::Uint8>(img.g), static_cast<sf::Uint8>(img.b) });

	sf::FloatRect	TextRect = Text.getLocalBounds();

	float	textX{ img.w / 2 - TextRect.width / 2 };
	float	textY{ img.h / 2 - TextRect.height / 2 - TextRect.top };

	Text.setPosition(textX, textY);

	BtnTexture.draw(Text);
	BtnTexture.display();

	img.texture = DD_NEW sf::Texture{ BtnTexture.getTexture() };

	img.sprite = DD_NEW sf::Sprite{ *img.texture };

	m_vButtonImage.push_back(img);
}

void	CSFMLRender::VCreateTextInputImage(int id, char *title, const std::string &text, int x, int y, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	_imgTextureSFML	texture{};

	texture.x = x;
	texture.y = y;
	texture.w = w;
	texture.h = h;
	texture.id = id;

	sf::RenderTexture	tempTexture{};

	tempTexture.create(texture.w, texture.h);
	tempTexture.clear(sf::Color{ static_cast<sf::Uint8>(r1), static_cast<sf::Uint8>(g1), static_cast<sf::Uint8>(b1) });

	if (title != nullptr)
	{
		RenderLine(&tempTexture, title, 8, 20, GetFont(fontType), GetFontSize(fontSize), r2, g2, b2);
	}

	if (text.length() > 0)
	{
		RenderLine(&tempTexture, text.c_str(), 8, h / 2, GetFont(fontType), GetFontSize(fontSize), r2, g2, b2);
	}

	tempTexture.display();

	texture.texture = DD_NEW sf::Texture{ tempTexture.getTexture() };
	texture.sprite = DD_NEW sf::Sprite{ *texture.texture };

	m_vTextInputImage.push_back(texture);
}

void	CSFMLRender::VCleanAllTextInputImages()
{
	for (size_t i = 0; i < m_vTextInputImage.size(); i++)
	{
		DD_DELETE(m_vTextInputImage[i].sprite);
		DD_DELETE(m_vTextInputImage[i].texture);
	}

	m_vTextInputImage.clear();
}

void	CSFMLRender::VCleanAllSelectBoxImages()
{
	for (size_t i = 0; i < m_vSelectBoxImage.size(); i++)
	{
		DD_DELETE(m_vSelectBoxImage[i].sprite);
		DD_DELETE(m_vSelectBoxImage[i].texture);
	}

	m_vSelectBoxImage.clear();
}

bool	CSFMLRender::VCleanSelectBoxImage(int id)
{
	if (m_vSelectBoxImage.size() < 1)
	{
		Log("CSFMLRender::VCleanSelectBoxImage() failed, there's no images");
		return	false;
	}

	for (int i = m_vSelectBoxImage.size() - 1; i >= 0; i--)
	{
		if (m_vSelectBoxImage[i].id == id)
		{
			DD_DELETE(m_vSelectBoxImage[i].sprite);
			DD_DELETE(m_vSelectBoxImage[i].texture);
			m_vSelectBoxImage.erase(m_vSelectBoxImage.begin() + i);
			return	true;
		}
	}

	Log("CSFMLRender::VCleanSelectBoxImage() failed, there's no image with id: ", id);

	return	false;
}

void	CSFMLRender::VRenderAllSelectBox()
{
	for (size_t i = 0; i < m_vSelectBoxImage.size(); i++)
	{
		RenderImage(m_vSelectBoxImage[i].x, m_vSelectBoxImage[i].y, m_vSelectBoxImage[i].sprite);
	}
}

void	CSFMLRender::VCreateSelectBoxImage(int id, const std::vector<std::string> &text, int x, int y, int startY, int sizeY, int w, int h, int r1, int g1, int b1, int r2, int g2, int b2, FontSize fontSize, FontType fontType, TextFlag::TextFlag flag, int x1, int x2)
{
	if (text.size() < 1)
		return;

	_imgTextureSFML	texture{};

	texture.x = x;
	texture.y = y;
	texture.w = w;
	texture.h = h;
	texture.id = id;

	sf::RenderTexture	tempTexture{};

	tempTexture.create(texture.w, texture.h);
	tempTexture.clear(sf::Color{ static_cast<sf::Uint8>(r1), static_cast<sf::Uint8>(g1), static_cast<sf::Uint8>(b1) });

	int	currentY = startY;

	for (size_t i = 0; i < text.size(); i++)
	{
		RenderLine(&tempTexture, text[i].c_str(), 0, currentY, GetFont(fontType), GetFontSize(fontSize), r2, g2, b2, flag, x1, x2);
		currentY += sizeY;
	}

	tempTexture.display();

	texture.texture = DD_NEW sf::Texture{ tempTexture.getTexture() };
	texture.sprite = DD_NEW sf::Sprite{ *texture.texture };

	m_vSelectBoxImage.push_back(texture);
}

bool	CSFMLRender::VCleanTextInputImage(int id)
{
	if (m_vTextInputImage.size() < 1)
	{
		Log("CSFMLRender::VCleanTextInputImage() failed, there's no images");
		return	false;
	}

	for (int i = m_vTextInputImage.size() - 1; i >= 0; i--)
	{
		if (m_vTextInputImage[i].id == id)
		{
			DD_DELETE(m_vTextInputImage[i].sprite);
			DD_DELETE(m_vTextInputImage[i].texture);
			m_vTextInputImage.erase(m_vTextInputImage.begin() + i);
			return	true;
		}
	}

	Log("CSFMLRender::VCleanTextInputImage() failed, there's no image with id: ", id);

	return false;
}

void	CSFMLRender::VCleanButtonImages()
{
	for (size_t i = 0; i < m_vButtonImage.size(); i++)
	{
		DD_DELETE(m_vButtonImage[i].sprite);
		DD_DELETE(m_vButtonImage[i].texture);
	}

	m_vButtonImage.clear();
}

bool	CSFMLRender::VCleanButtonImage(int id)
{
	if (m_vButtonImage.size() < 1)
	{
		Log("CSFMLRender::VCleanButtonImage() failed, size < 1");
		return	false;
	}

	for (int i = m_vButtonImage.size() - 1; i >= 0; i--)
	{
		if (m_vButtonImage[i].id == id)
		{
			DD_DELETE(m_vButtonImage[i].sprite);
			DD_DELETE(m_vButtonImage[i].texture);
			m_vButtonImage.erase(m_vButtonImage.begin() + i);
			return	true;
		}
	}

	Log("CSFMLRender::VCleanButtonImage() failed, there's no button with id: ", id);

	return false;
}

void	CSFMLRender::VRenderHUD()
{
	m_pWindow->draw(*m_pHUDSprite);

	if (m_hoverTextureReady)
		m_pWindow->draw(*m_pHoverSprite);
}

void	CSFMLRender::VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, bool callDisplay)
{
	m_pHUDTexture->clear(sf::Color{ 168, 237, 198 });

	RenderLine(m_pHUDTexture, std::to_string(frameRate).c_str(), 0, 8, m_pFont, 16, 100, 0, 100, TextFlag::alignRight, g_windowWidth);
	RenderLine(m_pHUDTexture, std::to_string(sec).c_str(), 0, 23, m_pFont, 16, 100, 0, 100, TextFlag::alignRight, g_windowWidth);
	RenderLine(m_pHUDTexture, std::to_string(mouseX).c_str(), g_windowWidth - 120, 40, m_pFont, 16, 100, 0, 100);
	RenderLine(m_pHUDTexture, std::to_string(mouseY).c_str(), g_windowWidth - 60, 40, m_pFont, 16, 100, 0, 100);

	if (callDisplay)
	{
		m_pHUDTexture->display();
	}
}

void	CSFMLRender::VCreateHUDImage(int frameRate, int sec, int mouseX, int mouseY, int imgIndex, int imgType)
{
	VCreateHUDImage(frameRate, sec, mouseX, mouseY, false); // false was passed because the HUD texture will be updated outside the called method

	if ((int)m_vImage[imgType].m_vTexture.size() <= imgIndex) // check if there's no such selected tile
	{
		m_pHUDTexture->display();
		return;
	}

	RenderLine(m_pHUDTexture, "Selected tile: ", 0, 10, m_pFont, 16, 0, 0, 0, TextFlag::alignRight, g_windowWidth / 2 + 130);

	RenderImage(g_windowWidth / 2 + 130, 8, m_pHUDTexture, m_vImage[imgType].m_vTexture[imgIndex].sprite);

	m_pHUDTexture->display();
}

void	CSFMLRender::VCreateInGameHUDImage(int frameRate, int sec, int mouseX, int mouseY, int hp, int mp)
{
	VCreateHUDImage(frameRate, sec, mouseX, mouseY, false);

	RenderLine(m_pHUDTexture, std::string{ "HP: " + std::to_string(hp) }.c_str(), 8, 8, m_pFont, 16, 0, 0, 255);
	RenderLine(m_pHUDTexture, std::string{ "MP: " + std::to_string(mp) }.c_str(), 8, 36, m_pFont, 16, 0, 0, 255);

	for (HudDataSFMLIterator it = m_hudData.begin(); it != m_hudData.end(); ++it)
	{
		m_pHUDTexture->draw(*it->second.sprite);
	}

	m_pHUDTexture->display();
}

void	CSFMLRender::VCreateInGameHoverData(const _inGameHoverData *hoverData)
{
	if (hoverData->data.size() < 1)
	{
		m_hoverTextureReady = false;
		return;
	}

	DD_DELETE(m_pHoverTexture);
	DD_DELETE(m_pHoverSprite);

	m_pHoverTexture = DD_NEW sf::RenderTexture{};
	m_pHoverTexture->create(hoverData->w, hoverData->h);

	m_pHoverSprite = DD_NEW sf::Sprite{ m_pHoverTexture->getTexture() };

	m_pHoverTexture->clear(sf::Color{ 168, 237, 198, 255 });

	m_pHoverSprite->setPosition(sf::Vector2f{ static_cast<float>(hoverData->posX), static_cast<float>(hoverData->posY) });

	if (hoverData->inGameHoverType == InGameHoverType::enemy || hoverData->inGameHoverType == InGameHoverType::hudSpell || hoverData->inGameHoverType == InGameHoverType::hudWeapAtt)
	{
		int y = 0;
		for (const auto& data : hoverData->data)
		{
			RenderLine(m_pHoverTexture, data.c_str(), 1, y, m_pFont, 12, 45, 55, 155, TextFlag::center, 0, hoverData->w);
			y += 12;
		}
	}

	m_pHoverTexture->display();

	m_hoverTextureReady = true;
}

void	CSFMLRender::VCreateInGameHUDData(RenderHudType hudType, const std::vector<int> &imgIndex, int x, int y, int w, int h, int offsetX)
{
	HudDataSFMLIterator it{};
	
	if ((it = m_hudData.find(hudType)) != m_hudData.end())
	{
		DD_DELETE(it->second.sprite);
		DD_DELETE(it->second.texture);
	}

	_imgRenderTextureSFMLReduced	&hud{ m_hudData[hudType] };
	
	hud.texture = DD_NEW sf::RenderTexture{};

	int width{ static_cast<int>(imgIndex.size() * offsetX) };

	hud.texture->create(width, h);

	hud.sprite = DD_NEW sf::Sprite{ hud.texture->getTexture() };

	hud.texture->clear(sf::Color{ 168, 237, 198, 255 });

	int posX{ 0 };

	int	imgType{}, newY{ 0 };

	if (hudType == RenderHudType::spells)
		imgType = TYPE_IMG_SPELL;
	else if (hudType == RenderHudType::weaponAttacks)
		imgType = TYPE_IMG_ITEM, newY = 32;

	for (size_t i = 0; i < imgIndex.size(); i++)
	{
		RenderImage(posX, y, imgIndex[i], imgType, hud.texture);
		posX += offsetX;
	}

	// rendering onto HUD so no need for g_windowHeight
	hud.sprite->setPosition(sf::Vector2f{ static_cast<float>(x), static_cast<float>(newY) });//+ g_windowHeight) });

	hud.texture->display();
}

void	CSFMLRender::VCreateGameMap(const _mapTileAll *map)
{
	DD_DELETE(m_pMapSprite);
	DD_DELETE(m_pMapTexture);

	m_pMapTexture = DD_NEW sf::RenderTexture{};

	if (map->currentMap == MapType::small_)
	{
		m_pMapTexture->create(MAP_WIDTH_SMALL, MAP_HEIGHT_SMALL);

		for (int i = 0; i < MAP_HEIGHT_SMALL / 32; i++)
		{
			for (int j = 0; j < MAP_WIDTH_SMALL / 32; j++)
			{
				RenderImage(j * 32, i * 32, map->small_->map[i][j].imgIndex, TYPE_IMG_FLOOR, m_pMapTexture);
			}
		}
	}
	else if (map->currentMap == MapType::medium)
	{
		m_pMapTexture->create(MAP_WIDTH_MEDIUM, MAP_HEIGHT_MEDIUM);

		for (int i = 0; i < MAP_HEIGHT_MEDIUM / 32; i++)
		{
			for (int j = 0; j < MAP_WIDTH_MEDIUM / 32; j++)
			{
				RenderImage(j * 32, i * 32, map->medium->map[i][j].imgIndex, TYPE_IMG_FLOOR, m_pMapTexture);
			}
		}
	}
	else if (map->currentMap == MapType::big)
	{
		m_pMapTexture->create(MAP_WIDTH_BIG, MAP_HEIGHT_BIG);

		for (int i = 0; i < MAP_HEIGHT_BIG / 32; i++)
		{
			for (int j = 0; j < MAP_WIDTH_BIG / 32; j++)
			{
				RenderImage(j * 32, i * 32, map->big->map[i][j].imgIndex, TYPE_IMG_FLOOR, m_pMapTexture);
			}
		}
	}

	m_pMapTexture->display();

	m_pMapSprite = DD_NEW sf::Sprite{ m_pMapTexture->getTexture() };
}

void	CSFMLRender::VRenderGameMap(const int cameraX, const int cameraY)
{
	RenderImage(cameraX - g_windowWidth / 2, cameraY - g_windowHeight / 2, g_windowWidth, g_windowHeight, 0, 0, m_pMapSprite);
}

bool	CSFMLRender::VValidId(int id_, const ImgTextureType &type)
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

bool	CSFMLRender::LoadImages()
{
	CSFMLImage	floorImg{};
	CSFMLImage	guiImg{};
	CSFMLImage	monstersImg{};
	CSFMLImage	spellsImg{};
	CSFMLImage	playerImg{};
	CSFMLImage	itemImg{};

	if (!floorImg.LoadImages(IMG_FLOOR_START, IMG_FLOOR_FINISH, "floor/"))
	{
		floorImg.Clean();
		Log("CSFMLRender::LoadImages() failed, Floor img load failed");
		return	false;
	}

	if (!guiImg.LoadImages(IMG_GUI_START, IMG_GUI_FINISH, "gui/"))
	{
		guiImg.Clean();
		Log("CSFMLRender::LoadImages() failed, GUI img load failed!");
		return	false;
	}

	if (!monstersImg.LoadImages(IMG_MONSTER_START, IMG_MONSTER_FINISH, "monster/"))
	{
		monstersImg.Clean();
		Log("CSFMLRender::LoadImages() failed, Monster img load failed");
		return	false;
	}

	if (!spellsImg.LoadImages(IMG_SPELL_START, IMG_SPELL_FINISH, "spell/"))
	{
		spellsImg.Clean();
		Log("CSFMLRender::LoadImages() failed, Spell img load failed");
		return	false;
	}

	if (!playerImg.LoadImages(IMG_PLAYER_START, IMG_PLAYER_FINISH, "player/"))
	{
		playerImg.Clean();
		Log("CSFMLRender::LoadImages() failed, Player img load failed");
		return	false;
	}

	if (!itemImg.LoadImages(IMG_ITEM_START, IMG_ITEM_FINISH, "item/"))
	{
		itemImg.Clean();
		Log("CSFMLRender::LoadImages() failed, Item img load failed");
		return	false;
	}

	// order matters, decided by IMG_TYPE_ in Const.h

	m_vImage.push_back(floorImg);
	m_vImage.push_back(guiImg);
	m_vImage.push_back(monstersImg);
	m_vImage.push_back(spellsImg);
	m_vImage.push_back(playerImg);
	m_vImage.push_back(itemImg);

	m_pHUDTexture = DD_NEW sf::RenderTexture{};
	m_pHUDTexture->create(g_windowWidth, g_windowHUDHeight);
	m_pHUDTexture->clear(sf::Color{ 168, 237, 198 });

	m_pHUDSprite = DD_NEW sf::Sprite{ m_pHUDTexture->getTexture() };

	m_pHUDSprite->setPosition(sf::Vector2f{ 0.f, static_cast<float>(g_windowHeight) });

	if (!DD::IO::LoadPassableFloor(nullptr, &m_vImage[0].m_vTexture))
	{
		Log("CSFMLRender::LoadImages() failed, DD::IO::LoadPassableFloor returned false");
		return	false;
	}

	return	true;
}

bool	CSFMLRender::LoadFonts()
{
	m_pFont = DD_NEW sf::Font();

	if (!m_pFont->loadFromFile("LiberationMono-Regular.ttf"))
	{
		Log("CSFMLRender::LoadFonts() failed, failed to load LiberationMono-Regular.ttf");
		return	false;
	}

	return	true;
}

sf::Font*	CSFMLRender::GetFont(const FontType &fontType)
{
	if (fontType == FontType::LiberationMono_Regular)
	{
		return	m_pFont;
	}

	Log("CSFMLRender::GetFont() failed, fontType: ", static_cast<int>(fontType));
	return	nullptr;
}

unsigned	int	CSFMLRender::GetFontSize(const FontSize &fontSize)
{
	if (fontSize == FontSize::font12)
		return	12;
	else if (fontSize == FontSize::font14)
		return	14;
	else if (fontSize == FontSize::font16)
		return	16;
	else if (fontSize == FontSize::font22)
		return	22;

	return	16;
}

void		CSFMLRender::RenderLine(sf::RenderTexture *des, const char *text, int x, const int y, sf::Font *Font, unsigned int FontSize, int r, int g, int b, TextFlag::TextFlag flag, int x1, int x2)
{
	sf::Text	Text{ std::string{ text }, *Font, FontSize };

	Text.setColor(sf::Color{ static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b) });

	if ((flag & TextFlag::nothing) == 0)
		AlignmentX(flag, x, x1, static_cast<int>(Text.getLocalBounds().width), x2);

	Text.setPosition(sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) });

	if (des == nullptr)
		m_pWindow->draw(Text);
	else
		des->draw(Text);
}

void	CSFMLRender::RenderImage(const int x, const int y, sf::Sprite *src)
{
	src->setPosition(sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) });

	m_pWindow->draw(*src);
}

void		CSFMLRender::RenderImageEx(const int x, const int y, const int w, const int h, double angle, float originX, float originY, sf::Sprite *sprite)
{
	// SFML does rotation around the top left corner, to change it a call to setOrigin is required
	// changing the origin will mess up with the positions 
	// for example, passed values for originX and Y are center positions, this means that x and y will not be top left corner positions anymore but center position
	// to change that we add originX and Y to the position so that its top left corner really will be x and y

	sprite->setPosition(sf::Vector2f{ static_cast<float>(x + originX), static_cast<float>(y + originY) });

	sprite->setOrigin(sf::Vector2f{ originX, originY });

	sprite->setRotation(static_cast<float>(angle));

	m_pWindow->draw(*sprite);

	sprite->setOrigin(sf::Vector2f{ 0, 0 });
}

void		CSFMLRender::RenderImage(const int x, const int y, sf::RenderTexture *des, sf::Sprite *src)
{
	src->setPosition(sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) });

	des->draw(*src);
}

void		CSFMLRender::RenderImage(int x, int y, int imgIndex, int imgType, sf::RenderTexture *des)
{
	RenderImage(x, y, des, m_vImage[imgType].m_vTexture[imgIndex].sprite);
}

void		CSFMLRender::RenderImage(int srcX, int srcY, int srcW, int srcH, int desX, int desY, sf::RenderTexture *des, sf::Sprite *src)
{
	src->setPosition(sf::Vector2f{ static_cast<float>(desX), static_cast<float>(desY) });
	src->setTextureRect(sf::IntRect{ srcX, srcY, srcW, srcH });

	des->draw(*src);
}

void		CSFMLRender::RenderImage(int srcX, int srcY, int srcW, int srcH, int desX, int desY, sf::Sprite *src)
{
	src->setPosition(sf::Vector2f{ static_cast<float>(desX), static_cast<float>(desY) });
	src->setTextureRect(sf::IntRect{ srcX, srcY, srcW, srcH });

	m_pWindow->draw(*src);
}