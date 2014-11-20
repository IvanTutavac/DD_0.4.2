#include "CSDLTimer.h"
#include <SDL2/SDL.h>

CSDLTimer::CSDLTimer()
{
	m_startTime = 0;
	m_endTime = 0;
	m_frequency = 0;

	m_frequency = SDL_GetPerformanceFrequency();
}

CSDLTimer::~CSDLTimer()
{

}

void	CSDLTimer::Start()
{
	m_startTime = SDL_GetPerformanceCounter();
}

void	CSDLTimer::End()
{
	m_endTime = SDL_GetPerformanceCounter();
}

double	CSDLTimer::GetTime()
{
	Uint64	tmp = m_endTime - m_startTime;

	return	((double)tmp / (double)m_frequency);
}

double	CSDLTimer::GetTimeEx()
{
	Uint64 tmp = SDL_GetPerformanceCounter();

	return	((double)tmp * (1000000 / (double)m_frequency));
}

double CSDLTimer::GetMicro()
{
	Uint64 tmp = m_endTime - m_startTime;

	return	((double)tmp * (1000000 / (double)m_frequency));
}
