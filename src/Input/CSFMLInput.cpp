#include "CSFMLInput.h"
#include <SFML/Graphics.hpp>
#include "../Tools/debug.h"
#include "../Data/Globals.h"
#include "../Data/Enums.h"

CSFMLInput::CSFMLInput()
{
	m_pWindowHandle = nullptr;
	m_pEvent = nullptr;
}

CSFMLInput::~CSFMLInput()
{

}

bool	CSFMLInput::VInit()
{
	m_pEvent = DD_NEW sf::Event();

	//m_pWindowHandle->setKeyRepeatEnabled(false);

	sf::Mouse::setPosition(sf::Vector2i(320, 240));

	return	true;
}

void	CSFMLInput::VClean()
{
	DD_DELETE(m_pEvent);
}

void	CSFMLInput::VUpdateInputMsg(_inputMessage *msg)
{
	while (m_pWindowHandle->pollEvent(*m_pEvent))
	{
		if (m_pEvent->type == sf::Event::Closed)
		{
			m_quit = true;
		}
		else if (m_pEvent->type == sf::Event::TextEntered && m_catchingTextInput)
		{
			int unicode = m_pEvent->text.unicode;

			if (unicode < 128 &&  unicode != 8 && unicode != 13) // ASCII only and we do not want backspace and enter
			{
				WriteMsg(msg, std::string{ static_cast<char>(m_pEvent->text.unicode) }.c_str());
			}
		}
		else if (m_pEvent->type == sf::Event::KeyPressed)
		{
			switch (m_pEvent->key.code)
			{
			case	sf::Keyboard::Escape:
				m_quit = true;
			case	sf::Keyboard::BackSpace:
				WriteMsg(msg, m_mappedKey["Backspace"].k, InputState::pressed);
				break;
			case	sf::Keyboard::Return:
				WriteMsg(msg, m_mappedKey["Enter"].k, InputState::pressed);
				break;
			case	sf::Keyboard::Space:
				WriteMsg(msg, m_mappedKey["Space"].k, InputState::pressed);
				break;
			case	sf::Keyboard::Left:
				WriteMsg(msg, m_mappedLeftRight["Left"].lR, InputState::pressed);
				break;
			case	sf::Keyboard::Right:
				WriteMsg(msg, m_mappedLeftRight["Right"].lR, InputState::pressed);
				break;
			case	sf::Keyboard::Up:
				WriteMsg(msg, m_mappedUpDown["Up"].uD, InputState::pressed);
				break;
			case	sf::Keyboard::Down:
				WriteMsg(msg, m_mappedUpDown["Down"].uD, InputState::pressed);
				break;
			case	sf::Keyboard::W:
				WriteMsg(msg, m_mappedKey["W"].k, InputState::pressed);
				break;
			case	sf::Keyboard::D:
				WriteMsg(msg, m_mappedKey["D"].k, InputState::pressed);
				break;
			case	sf::Keyboard::A:
				WriteMsg(msg, m_mappedKey["A"].k, InputState::pressed);
				break;
			case	sf::Keyboard::S:
				WriteMsg(msg, m_mappedKey["S"].k, InputState::pressed);
				break;
			case	sf::Keyboard::Q:
				WriteMsg(msg, m_mappedKey["Q"].k, InputState::pressed);
				break;
			case	sf::Keyboard::E:
				WriteMsg(msg, m_mappedKey["E"].k, InputState::pressed);
				break;
			case	sf::Keyboard::I:
				WriteMsg(msg, m_mappedKey["I"].k, InputState::pressed);
				break; 
			case	sf::Keyboard::C:
				WriteMsg(msg, m_mappedKey["C"].k, InputState::pressed);
				break;
			case	sf::Keyboard::F:
				WriteMsg(msg, m_mappedKey["F"].k, InputState::pressed);
				break;
			case	sf::Keyboard::G:
				WriteMsg(msg, m_mappedKey["G"].k, InputState::pressed);
				break;
			case	sf::Keyboard::Num1:
				WriteMsg(msg, m_mappedKey["one"].k, InputState::pressed);
				break;
			case	sf::Keyboard::Num2:
				WriteMsg(msg, m_mappedKey["two"].k, InputState::pressed);
				break;
			default:
				break;
			}
		}
		else if (m_pEvent->type == sf::Event::KeyReleased)
		{
			switch (m_pEvent->key.code)
			{
			case	sf::Keyboard::BackSpace:
				WriteMsg(msg, m_mappedKey["Backspace"].k, InputState::released);
				break;
			case	sf::Keyboard::Return:
				WriteMsg(msg, m_mappedKey["Enter"].k, InputState::released);
				break;
			case	sf::Keyboard::Space:
				WriteMsg(msg, m_mappedKey["Space"].k, InputState::released);
				break;
			case	sf::Keyboard::Left:
				WriteMsg(msg, m_mappedLeftRight["Left"].lR, InputState::released);
				break;
			case	sf::Keyboard::Right:
				WriteMsg(msg, m_mappedLeftRight["Right"].lR, InputState::released);
				break;
			case	sf::Keyboard::Up:
				WriteMsg(msg, m_mappedUpDown["Up"].uD, InputState::released);
				break;
			case	sf::Keyboard::Down:
				WriteMsg(msg, m_mappedUpDown["Down"].uD, InputState::released);
				break;
			case	sf::Keyboard::W:
				WriteMsg(msg, m_mappedKey["W"].k, InputState::released);
				break;
			case	sf::Keyboard::D:
				WriteMsg(msg, m_mappedKey["D"].k, InputState::released);
				break;
			case	sf::Keyboard::A:
				WriteMsg(msg, m_mappedKey["A"].k, InputState::released);
				break;
			case	sf::Keyboard::S:
				WriteMsg(msg, m_mappedKey["S"].k, InputState::released);
				break;
			case	sf::Keyboard::Q:
				WriteMsg(msg, m_mappedKey["Q"].k, InputState::released);
				break;
			case	sf::Keyboard::E:
				WriteMsg(msg, m_mappedKey["E"].k, InputState::released);
				break;
			case	sf::Keyboard::I:
				WriteMsg(msg, m_mappedKey["I"].k, InputState::released);
				break;
			case	sf::Keyboard::C:
				WriteMsg(msg, m_mappedKey["C"].k, InputState::released);
				break;
			case	sf::Keyboard::F:
				WriteMsg(msg, m_mappedKey["F"].k, InputState::released);
				break;
			case	sf::Keyboard::G: 
				WriteMsg(msg, m_mappedKey["G"].k, InputState::released);
				break;
			case	sf::Keyboard::Num1:
				WriteMsg(msg, m_mappedKey["one"].k, InputState::released);
				break;
			case	sf::Keyboard::Num2:
				WriteMsg(msg, m_mappedKey["two"].k, InputState::released);
				break;
			default:
				break;
			}
		}
		else if (m_pEvent->type == sf::Event::MouseButtonPressed)
		{
			if (m_pEvent->mouseButton.button == sf::Mouse::Button::Left)
			{
				WriteMsg(msg, m_pEvent->mouseButton.x, m_pEvent->mouseButton.y, m_mappedMouse["LeftButton"].m, InputState::pressed);
			}
			else if (m_pEvent->mouseButton.button == sf::Mouse::Button::Right)
			{
				WriteMsg(msg, m_pEvent->mouseButton.x, m_pEvent->mouseButton.y, m_mappedMouse["RightButton"].m, InputState::pressed);
			}
		}
		else if (m_pEvent->type == sf::Event::MouseButtonReleased)
		{
			if (m_pEvent->mouseButton.button == sf::Mouse::Button::Left)
			{
				WriteMsg(msg, m_pEvent->mouseButton.x, m_pEvent->mouseButton.y, m_mappedMouse["LeftButton"].m, InputState::released);
			}
			else if (m_pEvent->mouseButton.button == sf::Mouse::Button::Right)
			{
				WriteMsg(msg, m_pEvent->mouseButton.x, m_pEvent->mouseButton.y, m_mappedMouse["RightButton"].m, InputState::released);
			}
		}
		else if (m_pEvent->type == sf::Event::MouseMoved)
		{
			WriteMsg(msg, m_pEvent->mouseMove.x, m_pEvent->mouseMove.y);
		}
		else if (m_pEvent->type == sf::Event::MouseWheelMoved)
		{
			WriteMsg(msg, m_pEvent->mouseWheel.delta);
		}
	}
}

void	CSFMLInput::VSetWindow(sf::RenderWindow *window)
{
	m_pWindowHandle = window;
}

void	CSFMLInput::VStartTextInput()
{
	if (!m_catchingTextInput)
	{
		m_catchingTextInput = true;
	}
}
void	CSFMLInput::VStopTextInput()
{
	if (m_catchingTextInput)
	{
		m_catchingTextInput = false;
	}
}

