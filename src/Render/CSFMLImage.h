#pragma once

#include <vector>

namespace sf
{
	class Texture;
	class Sprite;
}

struct _textureSFML
{
	int	w,  h;
	bool	passable;
	sf::Texture	*texture;
	sf::Sprite	*sprite;
};

class CSFMLImage
{
private:

	// Loads an image onto m_vTexture
	// returns false on error and writes to log
	bool	LoadImage(const char * const name);

public:

	// calls Clean
	// folderName inside the img folder
	// loads images from imgStart to imgFinish (included too)
	bool	LoadImages(const	int	imgStart, const int imgFinish,const char* const folderName);

	// deletes all images
	void	Clean();

public:

	std::vector<_textureSFML>	m_vTexture;

};
