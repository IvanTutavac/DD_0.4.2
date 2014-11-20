#include "CInput.h"
#include "../Tools/StringWrapper.h"
#include "../Tools/Functions.h"
#include "../IO/IOFunctions.h"
#include "../Data/Enums.h"

CInput::CInput()
{
	m_quit = false;
	m_catchingTextInput = false;

	InitEvents(); // default keys

	if (!DD::IO::LoadKeysConfiguration(m_mappedKey, m_mappedMouse, m_mappedLeftRight, m_mappedUpDown, m_keyboardEvent, m_mouseEvent, m_leftRightEvent, m_upDownEvent))
	{
		// failed to load key configuration, default keys needed
		InitDefaultKeyMapping();
	}
}

bool	CInput::IsOver()
{
	return	m_quit;
}

void	CInput::InitEvents()
{
	m_keyboardEvent["A"] = KeyboardEvents::A;
	m_keyboardEvent["S"] = KeyboardEvents::S;
	m_keyboardEvent["D"] = KeyboardEvents::D;
	m_keyboardEvent["F"] = KeyboardEvents::F;
	m_keyboardEvent["Q"] = KeyboardEvents::Q;
	m_keyboardEvent["W"] = KeyboardEvents::W;
	m_keyboardEvent["E"] = KeyboardEvents::E;
	m_keyboardEvent["C"] = KeyboardEvents::C;
	m_keyboardEvent["G"] = KeyboardEvents::G;
	m_keyboardEvent["I"] = KeyboardEvents::I;
	m_keyboardEvent["one"] = KeyboardEvents::one;
	m_keyboardEvent["two"] = KeyboardEvents::two;
	m_keyboardEvent["Backspace"] = KeyboardEvents::Backspace;
	m_keyboardEvent["Space"] = KeyboardEvents::Space;
	m_keyboardEvent["Enter"] = KeyboardEvents::Enter;
	m_keyboardEvent["LeftButton"] = KeyboardEvents::LeftButton;
	m_keyboardEvent["RightButton"] = KeyboardEvents::RightButton;

	m_leftRightEvent["Left"] = LeftRight::Left;
	m_leftRightEvent["Right"] = LeftRight::Right;

	m_upDownEvent["Up"] = UpDown::Up;
	m_upDownEvent["Down"] = UpDown::Down;

	m_mouseEvent["LeftButton"] = MouseEvents::LeftButton;
	m_mouseEvent["RightButton"] = MouseEvents::RightButton;
}

void	CInput::InitDefaultKeyMapping()
{
	// take string and event values from m_keyboardEvent and map it to m_mappedKey

	std::map<std::string, KeyboardEvents>::iterator itKey = m_keyboardEvent.begin();
	for (; itKey != m_keyboardEvent.end(); ++itKey)
	{
		if (itKey->first != "nothing" )
		{
			if (itKey->first == "S" ) // map for S left mouse button
			{
				m_mappedKey[itKey->first] = _mappedKey("LeftButton", m_keyboardEvent["LeftButton"]);
			}
			else if (itKey->first == "D") // map for D right mouse button
			{
				m_mappedKey[itKey->first] = _mappedKey("RightButton", m_keyboardEvent["RightButton"]);
			}
			else if (itKey->first != "LeftButton" && itKey->first != "RightButton")
			{
				m_mappedKey[itKey->first] = _mappedKey(itKey->first, itKey->second);
			}
		}
	}

	std::map<std::string, UpDown>::iterator itUD = m_upDownEvent.begin();
	for (; itUD != m_upDownEvent.end(); ++itUD)
	{
		if (itUD->first != "nothing")
		{
			m_mappedUpDown[itUD->first] = _mappedUpDown(itUD->first, itUD->second);
		}
	}

	std::map<std::string, LeftRight>::iterator itLR = m_leftRightEvent.begin();
	for (; itLR != m_leftRightEvent.end(); ++itLR)
	{
		if (itLR->first != "nothing")
		{
			m_mappedLeftRight[itLR->first] = _mappedLeftRight(itLR->first, itLR->second);
		}
	}

	std::map<std::string, MouseEvents>::iterator itMouse = m_mouseEvent.begin();
	for (; itMouse != m_mouseEvent.end(); ++itMouse)
	{
		if (itMouse->first != "nothing")
		{
			m_mappedMouse[itMouse->first] = _mappedMouse(itMouse->first, itMouse->second);
		}
	}
}

void	CInput::WriteMsg(_inputMessage *msg, int x, int y, MouseEvents mouse, InputState state)
{
	msg->x = x;
	msg->y = y;
	msg->mouse.mouse = mouse;
	msg->mouse.state = state;
}

void	CInput::WriteMsg(_inputMessage *msg, int x, int y)
{
	msg->x1 = x;
	msg->y1 = y;
}

void	CInput::WriteMsg(_inputMessage *msg, KeyboardEvents key, InputState state)
{
	msg->key.key = key;
	msg->key.state = state;
}

void	CInput::WriteMsg(_inputMessage *msg, UpDown uD, InputState state)
{
	msg->upDown.upDown = uD;
	msg->upDown.state = state;
}

void	CInput::WriteMsg(_inputMessage *msg, LeftRight lR, InputState state)
{
	msg->leftRight.leftRight = lR;
	msg->leftRight.state = state;
}

void	CInput::WriteMsg(_inputMessage *msg,const char *letter)
{
	DD_Strcpy(msg->letter, std::char_traits<char>::length(letter) + 1, letter);
	msg->written = true;
}

void	CInput::WriteMsg(_inputMessage *msg,const int wheelY)
{
	msg->y2 = wheelY;
}
