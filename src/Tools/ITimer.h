#pragma once

class ITimer
{
public:

	ITimer(){}
	virtual ~ITimer() {}

	virtual void	Start() = 0;
	virtual void	End() = 0;

	virtual double	GetTime() = 0;
	virtual double	GetTimeEx() = 0;
	virtual double  GetMicro() = 0;
};
