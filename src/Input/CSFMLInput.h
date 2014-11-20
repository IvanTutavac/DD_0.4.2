#pragma once

#include "CInput.h"

namespace sf
{
	class Event;
}

class CSFMLInput : public CInput
{
private:

	sf::Event *m_pEvent;

public:

	CSFMLInput();
	~CSFMLInput();

	bool	VInit() override;
	void	VClean() override;

public:

	void	VUpdateInputMsg(_inputMessage *msg) override;

	void	VSetWindow(sf::RenderWindow *window) override;

	void	VStartTextInput() override;
	void	VStopTextInput() override;
};
