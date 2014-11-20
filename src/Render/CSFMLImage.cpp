#include "CSFMLImage.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../Tools/Log.h"
#include "../Tools/debug.h"

bool	CSFMLImage::LoadImages(const int imgStart, const int imgFinish,const char* const folderName)
{
	Clean();

	std::string begin{ "img/" + std::string{ folderName } };

	for (int i = imgStart; i <= imgFinish; i++)
	{
		std::string	name{begin + std::to_string(i) + ".png"};

		if (!LoadImage(name.c_str()))
			return	false;
	}

	return	true;
}

void	CSFMLImage::Clean()
{
	for (size_t i = 0; i < m_vTexture.size(); i++)
	{
		DD_DELETE(m_vTexture[i].sprite);
		DD_DELETE(m_vTexture[i].texture);
	}

	m_vTexture.clear();
}

bool	CSFMLImage::LoadImage(const char *const name)
{
	m_vTexture.push_back(_textureSFML{});

	m_vTexture.back().texture = DD_NEW sf::Texture();

	if (!m_vTexture.back().texture->loadFromFile(name))
	{
		Log(std::string{ "CSFMLImage::LoadImage() failed, image: " + std::string{ name } }.c_str());
		return	false;
	}

	sf::Vector2f	size{ m_vTexture.back().texture->getSize()};

	m_vTexture.back().w = static_cast<int>(size.x);
	m_vTexture.back().h = static_cast<int>(size.y);

	m_vTexture.back().sprite = DD_NEW sf::Sprite(*m_vTexture.back().texture);

	return	true;
}
