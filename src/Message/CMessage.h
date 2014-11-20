#pragma once

const	int	MSG_ARRAY_LIMIT = 20;

struct _inputMessage;
enum class MsgToRender;
enum class MsgToInput;
enum class MsgToLogic;

class CMessageToLogic
{
	MsgToLogic		m_msgToLogic[MSG_ARRAY_LIMIT];
	int				m_msgToLogicIndex;

public:

	CMessageToLogic();
	~CMessageToLogic();

	// returns true if there's no message to logic
	bool	IsMsgToLogicEmpty();
	// sets default to MsgToLogic, but for set elements, resets the index used for access
	void	ClearMsgToLogic();
	// sets each element in MsgToLogic to default value, resets the index used for access
	void	ClearAllMsgToLogic();
};

class CMessage
{
	_inputMessage	*m_pInputMsg;

	CMessageToLogic	m_msgToLogic;

	MsgToRender		m_msgToRender[MSG_ARRAY_LIMIT];
	int				m_msgToRenderIndex; // ClearMsgToRender will set 0 ( each frame that is ). After each RenderSet msg it increases by one and is used to set MsgToRender and in a loop for reading

	MsgToInput		m_msgToInput;
	int				m_selectedImgIndex;

	bool			IsSetMsgToRender(const MsgToRender value_);
	void			AddMsgToRender(const MsgToRender value_);

public:

	CMessage();
	~CMessage();

	bool	Init();
	void	Clean();

public:

	_inputMessage*	GetInputMsg();
	const _inputMessage*	GetInputMsgC() const;

	// set clearAll false in main logic function
	// clearAll resets everything if true
	// if false, if pressed do not reset arrow keys and mouse pressed msg ( because we can process them each time during a frame )
	// released input will be reseted
	// ( for instance, event pressed left key, the character will move while the key is pressed, so we do not want to reset the input of pressed type)
	// x,y,x1,y1 always reset
	void	ClearInputMsg(bool clearAll);

	// render

	bool	IsMsgToRenderEmpty();
	// sets default to MsgToRender, but for set elements, resets the index used for access
	void	ClearMsgToRender(); 
	// sets each element in MsgToRender to default value, resets the index used for access
	void	ClearAllMsgToRender(); 

	bool	IsInitInGame();
	void	SetInitInGame();

	bool	IsInitMainMenu();
	void	SetInitMainMenu();

	bool	IsInitMapEditor();
	void	SetInitMapEditor();

	bool	IsInitMapEditorAllTiles();
	void	SetInitMapEditorAllTiles();

	bool	IsNewMapInGame();
	void	SetNewMapInGame();

	bool	IsStartWeaponMainAnimation();
	void	SetStartWeaponMainAnimation();

	bool	IsSetInGameHUDSpellData();
	void	SetInGameHUDSpellData();

	bool	IsSetInGameHUDWeapAttData();
	void	SetInGameHUDWeapAttData();

	// default value is zero ( zero is a black tile )
	int		GetSelectedImgIndex();
	void	SetSelectedImgIndex(int value);

	bool	IsCreateButtonMenu();
	void	SetCreateButtonMenu();

	bool	IsDestroyButtonMenu();
	void	SetDestroyButtonMenu();

	bool	IsCreateTextBox();
	void	SetCreateTextBox();

	bool	IsCreateSelectBox();
	void	SetCreateSelectBox();

	bool	IsCloseTextBox();
	void	SetCloseTextBox();

	bool	IsCreateAllTextBox();
	void	SetCreateAllTextBox();

	bool	IsCloseAllTextBox();
	void	SetCloseAllTextBox();

	bool	IsCloseAllSelectBox();
	void	SetCloseAllSelectBox();

	bool	IsUpdateTextBox();
	void	SetUpdateTextBox();

	bool	IsSetDestroyWidgets();
	void	SetDestroyWidgets();

	// input

	bool	IsMsgToInputEmpty();
	void	ClearMsgToInput();

	bool	IsStartTextInput();
	void	SetStartTextInput();

	bool	IsStopTextInput();
	void	SetStopTextInput();

	// logic

	CMessageToLogic*	GetMsgToLogic();
};

