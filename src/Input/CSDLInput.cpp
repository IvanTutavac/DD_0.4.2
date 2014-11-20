#include "CSDLInput.h"
#include <SDL2/SDL.h>
#include "../Tools/debug.h"
#include "../Data/Enums.h"

CSDLInput::CSDLInput()
{
	m_pEvent = nullptr;
}

CSDLInput::~CSDLInput()
{

}

bool	CSDLInput::VInit()
{
	m_pEvent = DD_NEW SDL_Event;

	return	true;
}

void	CSDLInput::VClean()
{
	DD_DELETE(m_pEvent);
}

void	CSDLInput::VUpdateInputMsg(_inputMessage *msg)
{
	while (SDL_PollEvent(m_pEvent))
	{
		if (m_pEvent->type == SDL_QUIT)
		{
			m_quit = true;
		}
		else if (m_pEvent->type == SDL_TEXTINPUT && m_catchingTextInput)
		{
			WriteMsg(msg, m_pEvent->text.text);
		}
		else if (m_pEvent->type == SDL_KEYDOWN)
		{
			switch (m_pEvent->key.keysym.scancode)
			{
			case	SDL_SCANCODE_ESCAPE:
				m_quit = true;
				break;
			case	SDL_SCANCODE_BACKSPACE:
				WriteMsg(msg, m_mappedKey["Backspace"].k, InputState::pressed);
				break;
			case	SDL_SCANCODE_SPACE:
				WriteMsg(msg, m_mappedKey["Space"].k, InputState::pressed);
				break;
			case	SDL_SCANCODE_RETURN:
				WriteMsg(msg, m_mappedKey["Enter"].k, InputState::pressed);
				break;
			case	SDL_SCANCODE_LEFT:
				WriteMsg(msg, m_mappedLeftRight["Left"].lR, InputState::pressed);//LeftRight::pressedLeft);
				break;
			case	SDL_SCANCODE_RIGHT:
				WriteMsg(msg, m_mappedLeftRight["Right"].lR, InputState::pressed);//LeftRight::pressedRight);
				break;
			case	SDL_SCANCODE_UP:
				WriteMsg(msg, m_mappedUpDown["Up"].uD, InputState::pressed);//UpDown::pressedUp);
				break;
			case	SDL_SCANCODE_DOWN:
				WriteMsg(msg, m_mappedUpDown["Down"].uD, InputState::pressed);//UpDown::pressedDown);
				break;
			case	SDL_SCANCODE_Q:
				WriteMsg(msg, m_mappedKey["Q"].k, InputState::pressed);//KeyboardEvents::pressedQ);
				break;
			case	SDL_SCANCODE_E:
				WriteMsg(msg, m_mappedKey["E"].k, InputState::pressed);//KeyboardEvents::pressedE);
				break;
			case	SDL_SCANCODE_W:
				WriteMsg(msg, m_mappedKey["W"].k, InputState::pressed);//KeyboardEvents::pressedW);
				break;
			case	SDL_SCANCODE_S:
				WriteMsg(msg, m_mappedKey["S"].k, InputState::pressed);//KeyboardEvents::pressedS);
				break;
			case	SDL_SCANCODE_A:
				WriteMsg(msg, m_mappedKey["A"].k, InputState::pressed);//KeyboardEvents::pressedA);
				break;
			case	SDL_SCANCODE_D:
				WriteMsg(msg, m_mappedKey["D"].k, InputState::pressed);//KeyboardEvents::pressedD);
				break;
			case	SDL_SCANCODE_F:
				WriteMsg(msg, m_mappedKey["F"].k, InputState::pressed);//KeyboardEvents::pressedF);
				break;
			case	SDL_SCANCODE_I:
				WriteMsg(msg, m_mappedKey["I"].k, InputState::pressed);//KeyboardEvents::pressedI);
				break;
			case	SDL_SCANCODE_C:
				WriteMsg(msg, m_mappedKey["C"].k, InputState::pressed);//KeyboardEvents::pressedC);
				break;
			case	SDL_SCANCODE_G:
				WriteMsg(msg, m_mappedKey["G"].k, InputState::pressed);//KeyboardEvents::pressedG);
				break;
			case	SDL_SCANCODE_1:
				WriteMsg(msg, m_mappedKey["one"].k, InputState::pressed);
				break;
			case SDL_SCANCODE_2:
				WriteMsg(msg, m_mappedKey["two"].k, InputState::pressed);
			default:
				break;
			}
		}
		else if (m_pEvent->type == SDL_KEYUP)
		{
			switch (m_pEvent->key.keysym.scancode)
			{
			case	SDL_SCANCODE_BACKSPACE:
				WriteMsg(msg, m_mappedKey["Backspace"].k, InputState::released);
				break;
			case	SDL_SCANCODE_SPACE:
				WriteMsg(msg, m_mappedKey["Space"].k, InputState::released);
				break;
			case	SDL_SCANCODE_RETURN:
				WriteMsg(msg, m_mappedKey["Enter"].k, InputState::released);
				break;
			case	SDL_SCANCODE_LEFT:
				WriteMsg(msg, m_mappedLeftRight["Left"].lR, InputState::released);//LeftRight::releasedLeft);
				break;
			case	SDL_SCANCODE_RIGHT:
				WriteMsg(msg, m_mappedLeftRight["Right"].lR, InputState::released);//LeftRight::releasedRight);
				break;
			case	SDL_SCANCODE_UP:
				WriteMsg(msg, m_mappedUpDown["Up"].uD, InputState::released);//UpDown::releasedUp);
				break;
			case	SDL_SCANCODE_DOWN:
				WriteMsg(msg, m_mappedUpDown["Down"].uD, InputState::released);//UpDown::releasedDown);
				break;
			case	SDL_SCANCODE_Q:
				WriteMsg(msg, m_mappedKey["Q"].k, InputState::released);//KeyboardEvents::releasedQ);
				break;
			case	SDL_SCANCODE_E:
				WriteMsg(msg, m_mappedKey["E"].k, InputState::released);//KeyboardEvents::releasedE);
				break;
			case	SDL_SCANCODE_W:
				WriteMsg(msg, m_mappedKey["W"].k, InputState::released);//KeyboardEvents::releasedW);
				break;
			case	SDL_SCANCODE_S:
				WriteMsg(msg, m_mappedKey["S"].k, InputState::released);//KeyboardEvents::releasedS);
				break;
			case	SDL_SCANCODE_A:
				WriteMsg(msg, m_mappedKey["A"].k, InputState::released);//KeyboardEvents::releasedA);
				break;
			case	SDL_SCANCODE_D:
				WriteMsg(msg, m_mappedKey["D"].k, InputState::released);//KeyboardEvents::releasedD);
				break;
			case	SDL_SCANCODE_F:
				WriteMsg(msg, m_mappedKey["F"].k, InputState::released);//KeyboardEvents::releasedF);
				break;
			case	SDL_SCANCODE_I:
				WriteMsg(msg, m_mappedKey["I"].k, InputState::released);//KeyboardEvents::releasedI);
				break;
			case	SDL_SCANCODE_C:
				WriteMsg(msg, m_mappedKey["C"].k, InputState::released);//KeyboardEvents::releasedC);
				break;
			case	SDL_SCANCODE_G:
				WriteMsg(msg, m_mappedKey["G"].k, InputState::released);//KeyboardEvents::releasedG);
				break;
			case SDL_SCANCODE_1:
				WriteMsg(msg, m_mappedKey["one"].k, InputState::released);
				break;
			case SDL_SCANCODE_2:
				WriteMsg(msg, m_mappedKey["two"].k, InputState::released);
				break;
			default:
				break;
			}
		}
		else if (m_pEvent->type == SDL_MOUSEBUTTONDOWN)
		{
			if (m_pEvent->button.button == SDL_BUTTON_LEFT)
				WriteMsg(msg, m_pEvent->button.x, m_pEvent->button.y, m_mappedMouse["LeftButton"].m, InputState::pressed);//MouseEvents::pressedLeftButton);
			else if (m_pEvent->button.button == SDL_BUTTON_RIGHT)
				WriteMsg(msg, m_pEvent->button.x, m_pEvent->button.y, m_mappedMouse["RightButton"].m, InputState::pressed);//MouseEvents::pressedRightButton);
		}
		else if (m_pEvent->type == SDL_MOUSEBUTTONUP)
		{
			if (m_pEvent->button.button == SDL_BUTTON_LEFT)
				WriteMsg(msg, m_pEvent->button.x, m_pEvent->button.y, m_mappedMouse["LeftButton"].m, InputState::released);//MouseEvents::releasedLeftButton);
			else if (m_pEvent->button.button == SDL_BUTTON_RIGHT)
				WriteMsg(msg, m_pEvent->button.x, m_pEvent->button.y, m_mappedMouse["RightButton"].m, InputState::released);//MouseEvents::releasedRightButton);
		}
		else if (m_pEvent->type == SDL_MOUSEMOTION)
		{
			WriteMsg(msg, m_pEvent->motion.x, m_pEvent->motion.y);
		}
		else if (m_pEvent->type == SDL_MOUSEWHEEL)
		{
			WriteMsg(msg, m_pEvent->wheel.y);
		}
	}
}

void	CSDLInput::VStartTextInput()
{
	if (!m_catchingTextInput)
	{
		SDL_StartTextInput();

		m_catchingTextInput = true;
	}
}
void	CSDLInput::VStopTextInput()
{
	if (m_catchingTextInput)
	{
		SDL_StopTextInput();

		m_catchingTextInput = false;
	}
}
