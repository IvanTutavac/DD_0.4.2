#include <SDL2/SDL_image.h>
#include "CSDLImage.h"
#include "../Tools/Log.h"
#include "../Tools/StringWrapper.h"

SDL_Surface *LoadSurface(const char *name)
{
	SDL_Surface *surface{ nullptr };

	surface = IMG_Load(name);

	return	surface;
}

bool	CSDLImage::Init(SDL_Renderer *Renderer, const int imgStart, const int imgFinish,const char * const folder)
{
	Clean();

	m_n = imgStart;
	char buffer[64] = "", name[64] = "", a[] = "img/",b[64] = "";

	DD_Strcat(b,sizeof(b), a);

	if (folder != nullptr)
	{
		DD_Strcat(b,sizeof(b), folder);
	}

	for (int i = imgStart; i <= imgFinish; i++)
	{
		DD_Strcpy(name,sizeof(name), b);
		DD_Itoa(i, buffer,sizeof(buffer), 10);
		DD_Strcat(name,sizeof(name), buffer);
		DD_Strcat(name,sizeof(name), ".png");

		if (!CreateTexture(name, Renderer))
		{
			return	false;
		}

		buffer[0] = 0;
		name[0] = 0;
	}

	return	true;
}

void	CSDLImage::Clean()
{
	for (size_t i = 0; i < m_vTexture.size(); i++)
	{
		SDL_DestroyTexture(m_vTexture[i].texture);
	}

	m_vTexture.clear();
}

bool	CSDLImage::CreateTexture(const char *name, SDL_Renderer *Renderer)
{
	SDL_Surface *surface = nullptr;

	surface = IMG_Load(name);

	if (surface == nullptr)
	{
		WriteToLog(LogSurface);

		return	false;
	}

	_texture	texture;

	texture.w = surface->w;
	texture.h = surface->h;

	texture.texture = SDL_CreateTextureFromSurface(Renderer, surface);

	if (texture.texture == nullptr)
	{
		WriteToLog(LogTexture);

		return	false;
	}

	m_vTexture.push_back(texture);

	SDL_FreeSurface(surface);

	m_n++;

	return	true;
}

void	CSDLImage::WriteToLog(const int log)
{
	char text[32];

	DD_Itoa(m_n, text,sizeof(text), 10);

	if (log == LogSurface)
	{
		DD_Strcat(text,sizeof(text), ".img failed to load");
	}
	else if (log == LogTexture)
	{
		DD_Strcat(text,sizeof(text), ".texture was not created");
	}

	Log(text);
}
