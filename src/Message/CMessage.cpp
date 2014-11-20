#include "CMessage.h"
#include "MessageData.h"
#include "../Tools/debug.h"
#include "../Input/InputData.h"
#include "../Data/Enums.h"

CMessage::CMessage()
{
	m_pInputMsg = nullptr;
}

CMessage::~CMessage()
{

}

bool	CMessage::Init()
{
	m_pInputMsg =  DD_NEW _inputMessage;

	ClearInputMsg(true);

	ClearAllMsgToRender();
	m_msgToLogic.ClearAllMsgToLogic();

	ClearMsgToInput();

	m_selectedImgIndex = 0;

	return	true;
}

void	CMessage::Clean()
{
	DD_DELETE(m_pInputMsg);
}

CMessageToLogic*	CMessage::GetMsgToLogic()
{
	return &m_msgToLogic;
}

_inputMessage*	CMessage::GetInputMsg()
{
	return	m_pInputMsg;
}

const _inputMessage*	CMessage::GetInputMsgC() const
{
	return	m_pInputMsg;
}

void	CMessage::ClearInputMsg(bool clearAll)
{
	m_pInputMsg->x = 0, m_pInputMsg->y = 0; 
	//m_pInputMsg->x1 = 0, m_pInputMsg->y1 = 0; // <- we do not want to clear mouse movement msg...
	m_pInputMsg->written = false;
	m_pInputMsg->y2 = 0;

	if (clearAll == false) // we must reset released input
	{
		if (m_pInputMsg->mouse.state == InputState::released)
		{
			m_pInputMsg->mouse.state = InputState::nothing;
			m_pInputMsg->mouse.mouse = MouseEvents::nothing;
		}

		if (m_pInputMsg->leftRight.state == InputState::released)
		{
			m_pInputMsg->leftRight.state = InputState::nothing;
			m_pInputMsg->leftRight.leftRight = LeftRight::nothing;
		}

		if (m_pInputMsg->upDown.state == InputState::released)
		{
			m_pInputMsg->upDown.state = InputState::nothing;
			m_pInputMsg->upDown.upDown = UpDown::nothing;
		}

		if (m_pInputMsg->key.state == InputState::released)
		{
			m_pInputMsg->key.key = KeyboardEvents::nothing;
			m_pInputMsg->key.state = InputState::nothing;
		}
	}
	else  // we reset everything
	{
		m_pInputMsg->mouse.mouse = MouseEvents::nothing;
		m_pInputMsg->mouse.state = InputState::nothing;

		m_pInputMsg->leftRight.leftRight = LeftRight::nothing;
		m_pInputMsg->leftRight.state = InputState::nothing;

		m_pInputMsg->upDown.upDown = UpDown::nothing;
		m_pInputMsg->upDown.state = InputState::nothing;
	
		m_pInputMsg->key.key = KeyboardEvents::nothing;
		m_pInputMsg->key.state = InputState::nothing;
	}
}

void	CMessage::ClearMsgToRender()
{
	for (int i = 0; i < m_msgToRenderIndex; i++)
	{
		m_msgToRender[i] = MsgToRender::nothing;
	}

	m_msgToRenderIndex = 0;
}

void	CMessage::ClearAllMsgToRender()
{
	for (int i = 0; i < MSG_ARRAY_LIMIT; i++)
	{
		m_msgToRender[i] = MsgToRender::nothing;
	}

	m_msgToRenderIndex = 0;
}

bool	CMessage::IsMsgToRenderEmpty()
{
	if (m_msgToRenderIndex == 0) // no set method was called, thats why it's zero
		return	true;
	return	false;
	/*
	int j = 0;
	for (int i = 0; i < m_msgToRenderIndex; i++)
	{
		if (m_msgToRender[i] == MsgToRender::nothing)
			j++;
	}

	if (j == m_msgToRenderIndex) // testirati !!!!
		return	true;

	return	false;*/
}

bool	CMessage::IsInitInGame()
{
	for (int i = 0; i < m_msgToRenderIndex; i++)
	{
		if (m_msgToRender[i] == MsgToRender::initInGame)
			return	true;
	}

	return	false;
}

void	CMessage::SetInitInGame()
{
	AddMsgToRender(MsgToRender::initInGame);
}

bool	CMessage::IsInitMainMenu()
{
	return	IsSetMsgToRender(MsgToRender::initMainMenu);
}

void	CMessage::SetInitMainMenu()
{
	AddMsgToRender(MsgToRender::initMainMenu);
}

bool	CMessage::IsInitMapEditor()
{
	return	IsSetMsgToRender(MsgToRender::initMapEditor);
}

void	CMessage::SetInitMapEditor()
{
	AddMsgToRender(MsgToRender::initMapEditor);
}

bool	CMessage::IsInitMapEditorAllTiles()
{
	return	IsSetMsgToRender(MsgToRender::initMapEditorAllTiles);
}

void	CMessage::SetInitMapEditorAllTiles()
{
	AddMsgToRender(MsgToRender::initMapEditorAllTiles);
}

bool	CMessage::IsNewMapInGame()
{
	return	IsSetMsgToRender(MsgToRender::startNewMapInGame);
}

void	CMessage::SetNewMapInGame()
{
	AddMsgToRender(MsgToRender::startNewMapInGame);
}

bool	CMessage::IsStartWeaponMainAnimation()
{
	return	IsSetMsgToRender(MsgToRender::startWeaponMainAnimation);
}

void	CMessage::SetStartWeaponMainAnimation()
{
	AddMsgToRender(MsgToRender::startWeaponMainAnimation);
}

bool	CMessage::IsSetInGameHUDSpellData()
{
	return	IsSetMsgToRender(MsgToRender::setInGameHUDSpellData);
}

void	CMessage::SetInGameHUDSpellData()
{
	AddMsgToRender(MsgToRender::setInGameHUDSpellData);
}

bool	CMessage::IsSetInGameHUDWeapAttData()
{
	return	IsSetMsgToRender(MsgToRender::setInGameHUDWeapAttData);
}

void	CMessage::SetInGameHUDWeapAttData()
{
	return	AddMsgToRender(MsgToRender::setInGameHUDWeapAttData);
}

int		CMessage::GetSelectedImgIndex()
{
	return	m_selectedImgIndex;
}

void	CMessage::SetSelectedImgIndex(int value)
{
	m_selectedImgIndex = value;
}

bool	CMessage::IsCreateButtonMenu()
{
	return	IsSetMsgToRender(MsgToRender::createButtonMenu);
}

void	CMessage::SetCreateButtonMenu()
{
	AddMsgToRender(MsgToRender::createButtonMenu);
}

bool	CMessage::IsCreateSelectBox()
{
	return	IsSetMsgToRender(MsgToRender::createSelectBox);
}

void	CMessage::SetCreateSelectBox()
{
	AddMsgToRender(MsgToRender::createSelectBox);
}

bool	CMessage::IsCreateTextBox()
{
	return	IsSetMsgToRender(MsgToRender::createTextBox);
}

void	CMessage::SetCreateTextBox()
{
	AddMsgToRender(MsgToRender::createTextBox);
}

bool	CMessage::IsCloseTextBox()
{
	return	IsSetMsgToRender(MsgToRender::closeTextBox);
}

void	CMessage::SetCloseTextBox()
{
	AddMsgToRender(MsgToRender::closeTextBox);
}

bool	CMessage::IsCreateAllTextBox()
{
	return	IsSetMsgToRender(MsgToRender::createAllTextBox);
}

void	CMessage::SetCreateAllTextBox()
{
	AddMsgToRender(MsgToRender::createAllTextBox);
}

void	CMessage::SetCloseAllTextBox()
{
	AddMsgToRender(MsgToRender::closeAllTextBox);
}

bool	CMessage::IsCloseAllTextBox()
{
	return	IsSetMsgToRender(MsgToRender::closeAllTextBox);
}

void	CMessage::SetCloseAllSelectBox()
{
	AddMsgToRender(MsgToRender::closeAllSelectBox);
}

bool	CMessage::IsCloseAllSelectBox()
{
	return	IsSetMsgToRender(MsgToRender::closeAllSelectBox);
}

bool	CMessage::IsDestroyButtonMenu()
{
	for (int i = 0; i < m_msgToRenderIndex; i++)
	{
		if (m_msgToRender[i] == MsgToRender::closeButtonMenu)
			return	true;
	}

	return	false;
}

void	CMessage::SetDestroyButtonMenu()
{
	AddMsgToRender(MsgToRender::closeButtonMenu);
}

bool	CMessage::IsMsgToInputEmpty()
{
	if (m_msgToInput == MsgToInput::nothing)
		return	true;
	return	false;
}

void	CMessage::ClearMsgToInput()
{
	m_msgToInput = MsgToInput::nothing;
}

bool	CMessage::IsStartTextInput()
{
	if (m_msgToInput == MsgToInput::startTextInput)
		return	true;
	return	false;
}

void	CMessage::SetStartTextInput()
{
	m_msgToInput = MsgToInput::startTextInput;
}

bool	CMessage::IsStopTextInput()
{
	if (m_msgToInput == MsgToInput::stopTextInput)
		return	true;
	return	false;
}

void	CMessage::SetStopTextInput()
{
	m_msgToInput = MsgToInput::stopTextInput;
}

bool	CMessage::IsUpdateTextBox()
{
	return	IsSetMsgToRender(MsgToRender::updateTextBox);
}

void	CMessage::SetUpdateTextBox()
{
	AddMsgToRender(MsgToRender::updateTextBox);
}

void	CMessage::SetDestroyWidgets()
{
	AddMsgToRender(MsgToRender::destroyWidgets);
}

bool	CMessage::IsSetDestroyWidgets()
{
	return	IsSetMsgToRender(MsgToRender::destroyWidgets);
}

void	CMessage::AddMsgToRender(const MsgToRender value_)
{
	m_msgToRender[m_msgToRenderIndex] = value_;
	m_msgToRenderIndex++;
}

bool	CMessage::IsSetMsgToRender(const MsgToRender value_)
{
	for (int i = 0; i < m_msgToRenderIndex; i++)
	{
		if (m_msgToRender[i] == value_)
			return	true;
	}

	return	false;
}

// logic msg

CMessageToLogic::CMessageToLogic()
{

}

CMessageToLogic::~CMessageToLogic()
{

}

void	CMessageToLogic::ClearMsgToLogic()
{
	for (int i = 0; i < m_msgToLogicIndex; i++)
	{
		m_msgToLogic[i] = MsgToLogic::nothing;
	}

	m_msgToLogicIndex = 0;
}

void	CMessageToLogic::ClearAllMsgToLogic()
{
	for (int i = 0; i < MSG_ARRAY_LIMIT; i++)
	{
		m_msgToLogic[i] = MsgToLogic::nothing;
	}

	m_msgToLogicIndex = 0;
}

bool	CMessageToLogic::IsMsgToLogicEmpty()
{
	if (m_msgToLogicIndex == 0)
		return	true;
	return	false;
}

// end of logic msg

