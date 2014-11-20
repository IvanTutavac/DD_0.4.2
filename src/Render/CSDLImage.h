#pragma once

#include <vector>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Surface;

const int LogSurface = 0;
const int LogTexture = 1;

struct _texture
{
	int w, h;
	bool	passable;
	SDL_Texture *texture;
};

SDL_Surface *LoadSurface(const char *name);

class CSDLImage
{
private:

	int m_n; // used for log, which img

private:

	bool	CreateTexture(const char *name,SDL_Renderer *Renderer);
	void	WriteToLog(const int log);

public:

	// calls Clean
	// load files from imgStart.png till imgFinish.png (imgFinish will be loaded too) into m_vTexture 
	// if you want images from img/ just pass nullptr as folder
	// for folder use folderName/  wall/ for example 
	bool	Init(SDL_Renderer *Renderer, const int imgStart, const int imgFinish,const char * const folder);

	// deletes image files and clears the vector
	void	Clean();

public:

	std::vector<_texture> m_vTexture; // contains image files
};
