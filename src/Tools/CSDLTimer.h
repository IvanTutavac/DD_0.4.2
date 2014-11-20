#pragma once

#include "ITimer.h"

class CSDLTimer : public ITimer
{
	unsigned long long	int	m_startTime;
	unsigned long long	int	m_endTime;
	unsigned long long	int	m_frequency;

public:

	CSDLTimer();
	~CSDLTimer();

	// sets m_startTime
	void	Start() override;
	// sets m_endTime
	void	End()	override;

	// logic time, time difference between Start() and End(), how much time has passed between calls to those two functions
	double	GetTime() override;
	// for locking, get the time passed
	double	GetTimeEx() override;
	// fps time, time difference between Start() and End() in microseconds
	double	GetMicro()	override;
};