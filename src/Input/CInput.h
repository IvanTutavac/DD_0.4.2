#pragma once

#include "InputData.h"
#include <map>

namespace sf
{
	class RenderWindow;
};

class CInput
{
protected:

	bool	m_quit;
	bool	m_catchingTextInput; // default false, set by VStop and Start text input, ensures that multiple calls to the same function only take action on the first call

	sf::RenderWindow	*m_pWindowHandle; // sfml needs this for events ...

	// used for getting events 
	std::map<std::string, KeyboardEvents> m_keyboardEvent;
	std::map<std::string, MouseEvents> m_mouseEvent;
	std::map<std::string, LeftRight> m_leftRightEvent;
	std::map<std::string, UpDown>	m_upDownEvent;

	// used for selecting which event should be used for WriteMsg ( mappedKey will be used for the key pressed )

	std::map<std::string, _mappedKey> m_mappedKey;
	std::map<std::string, _mappedMouse> m_mappedMouse;
	std::map<std::string, _mappedLeftRight> m_mappedLeftRight;
	std::map<std::string, _mappedUpDown> m_mappedUpDown;

protected:

	void	WriteMsg(_inputMessage *msg, int x, int y, MouseEvents mouse, InputState state);
	void	WriteMsg(_inputMessage *msg, int x, int y);
	void	WriteMsg(_inputMessage *msg, KeyboardEvents key, InputState state);
	void	WriteMsg(_inputMessage *msg, UpDown uD, InputState state);
	void	WriteMsg(_inputMessage *msg, LeftRight lR, InputState state);
	void	WriteMsg(_inputMessage *msg,const char *letter);
	void	WriteMsg(_inputMessage *msg,const int wheelY);

	// inserts default values into m_keyboardEvent, m_mouseEvent, m_leftRightEvent, m_upDownEvent
	// left mouse and right mouse will also be mapped to s and d
	void	InitEvents();

	// m_mappedKey, m_mappedMouse, m_mappedLeftRight, m_mappedUpDown will be initialized 
	void	InitDefaultKeyMapping();

public:

	// takes care of key configuration
	CInput(); 
	virtual ~CInput(){}

	virtual bool	VInit() = 0;
	virtual void	VClean() = 0;

public:

	virtual void	VUpdateInputMsg(_inputMessage *msg) = 0;

	virtual void	VSetWindow(sf::RenderWindow *window) {}

	virtual void	VStartTextInput() = 0;
	virtual void	VStopTextInput() = 0;

	bool	IsOver();
};
