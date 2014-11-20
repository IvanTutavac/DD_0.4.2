#pragma once

#include "CInput.h"

typedef union SDL_Event SDL_Event;

class CSDLInput : public CInput
{
private:

	SDL_Event *m_pEvent;

public:

	CSDLInput();
	~CSDLInput();

	bool	VInit() override;
	void	VClean() override;

public:

	void	VUpdateInputMsg(_inputMessage *msg) override;

	void	VStartTextInput() override;
	void	VStopTextInput() override;
};