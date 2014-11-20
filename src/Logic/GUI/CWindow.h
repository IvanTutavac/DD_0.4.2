#pragma once

enum class WindowType;
enum class WidgetAction;
enum class WidgetType;
struct _mouseEvents;
struct _keyboardEvents;
enum class InputState;
struct _widgetResponse;
struct _textBoxWrapper;
struct _buttonWrapper;
struct _menuWrapper;
struct _selectBoxWrapper;
struct _GUITypeIdWrapper;

class CGUIAction;
class CTextBox;
class CMenu;
class CWidget;
class CSelectBox;

class CWindow
{
private:

	// when adding new widgets, one needs to update GetWidgetIndex(), GetWidgetID(), CGUI::OpenChildWidget(), OpenWidget(), GetWidget()
	// CWindow::Clear() and construct a open method for new widget
	// after that, update the OpenWidget() method in GUI and CRenderManager::DestroyWidgets()

	_textBoxWrapper	*m_pTextBox; // collection of text boxes
	_buttonWrapper	*m_pButton; // collection of buttons 
	_menuWrapper	*m_pMenu; // collection of menus
	_selectBoxWrapper *m_pSelectBox; // collection of selectBoxes
	_GUITypeIdWrapper *m_pWidgetsClean; // used for widgets clean up
	// trebalo bi napraviti funkciju na widgetu koja isto kao sto i gui radi
	// cisti sve u widgetu na default stanje
	// ono sto gui radi pri promjeni prozora prebaciti u window->clear()

	bool	InitOptionsWindow();
	bool	InitMainMenuWindow();
	bool	InitInGameWindow();
	bool	InitMapEditorWindow();
	bool	InitMapEditorImageSelectionWindow();

	int		m_pressedWidgetID; // used to check whether a release event happened on the same widget that fired a press event

	// updated by ReadResponse()
	int			m_childWidgetID; // id of the child of the widget that fired the event
	int			m_parentID; // id of the widget that fired the event
	WidgetType	m_childWidgetType; // type of the child of the widget that fired the event
	WidgetType  m_parentWidgetType; // type of the widget that fired the event

	int			m_activeTextBoxID; // id of the textBox recieving input (set by clicking on a textbox, clicking something else sets it to -1)
	int			m_activeTextBoxIndex;

	int			m_activeSelectBoxID; // id of the clicked selectBox, so that render can know which selectBox is being scrolled (window too) (clicking something else sets it to -1)
	int			m_activeSelectBoxIndex;

	// writes to action by reading response
	// returns 0 if no update happened 
	// returns 1 if update happened
	// returns -1 upon error
	int	UpdateActions(_widgetResponse &response, const InputState &inputState, CGUIAction *guiAction);

	// reads response sent by a widget and updates window data
	bool	ReadResponse(const _widgetResponse &response);

	// returns -1 for not found, -2 for invalid arguments, -3 for widget of passed widgetType not existing or >= 0 for valid id 
	int		GetWidgetID(const char *WidgetName_, const WidgetType &widgetType_);

	// update the childID member of parent with the id of a widget that has a name equal to childName
	// also update the childWidgetType with childType
	// returns false on error and writes to log
	bool	SetWidgetChild(CWidget *parent,const char *childName, const WidgetType &childType);

	// returns >= 0 if a widget exists 
	// returns -1 on error and writes to log 
	// index used for:  std::vector<CWidget> Widget; Widget[index]
	int		GetWidgetIndex(const int widgetID, const WidgetType &type);

	// called by OpenChildWidget
	// calls a method to open a widget
	// return its value in OpenChildWidget
	int		OpenWidget(WidgetType &type,int index_);

	// Widget[index_]  -> position of the widget that needs to be opened
	// returns 1 if the widget has been shown 
	// returns 0 if the widget is already visible, no need to send a message to render to create the widget
	// The menu's parent will get the openedChildID updated with the newly opened menu widgetID
	// the child will also contain the id of the parent
	// method that calls this one needs to check if the index is valid
	int		OpenMenu(int index_);

	// Widget[index_]  -> position of the widget that needs to be opened
	// sets activeTextBox id and index (render needs those to access the text box), will be reset upon clicking something else
	// returns 1 if the widget has been shown 
	// returns 0 if the widget is already visible, no need to send a message to render to create the widget
	// method that calls this one needs to check if the index is valid
	int		OpenTextBox(int index_);

	// Widget[index_]  -> position of the widget that needs to be opened
	// sets activeSelectBox id and index (render needs those to access the text box)
	// returns 1 if the widget has been shown 
	// returns 0 if the widget is already visible, no need to send a message to render to create the widget
	// method that calls this one needs to check if the index is valid
	int		OpenSelectBox(int index_);

	CWidget*	GetWidget(int index, const WidgetType &type);

	CWidget*	GetButtonInsideMenu(const int buttonID);

	// find every child ( parent included !, parent is the first child), hide it and write its position and type to m_pWidgetsClean
	// returns false on error ( usually a widget has a childID, but the child hasn't been found, writes to log
	bool		HideAllChildren(CWidget *parent,const int index,const WidgetType type);

	// widgetID -> childID, will be used to found the parent of the widget menu
	// Hides a menu which is already being shown (if there's one) for that parent
	// Returns -1 upon error
	// Returns 0 if there's no need to reopen the widget, OpenWidget should not be called 
	// Returns 1 otherwise
	int		HideAllMenuButtons(const int widgetID);

	// searches for visible menus and hides children of the buttons inside if the child is not a menu
	void	HideAllMenuButtonsChild();

	// will hide all Menu buttons which are visible, calls VReset() (to preserve menus which are visible by default)
	void		HideAllMenuButtons();

public:

	CWindow();
	~CWindow();

	// upon error will call Clean()
	bool	Init(const WindowType &type);
	void	Clean();

	// reset widgets 
	// called when changing windows
	void	Reset(); 

public:

	// main function used for checking what to do with user input
	// widgetAction will be updated
	bool	ProcessInput(const _mouseEvents &mouseEvent, const _keyboardEvents &key, const int mouseX, const int mouseY, const int mouseX1, const int mouseY1,const int mouseWheelY, CGUIAction *guiAction);

	// returns true if we can set a tile 
	// used because a menu or other widgets can be over the map, so it checks whether a mouse movement is valid for setting tile
	// widget positioning will not work because pressed and mouseX1, mouseY1 in message are not being reset on each frame, so use this method
	// widget positioning (ProcessInput above setting tile, if no action set tile, will not work because of the above)
	bool	IsValidMapEditorSetTile(const int mouseX1, const int mouseY1);

	// updates type with the type of the newly created widget
	// returns -1 on error and writes to log  ( methods inside it )
	// returns 0 if a child has already been opened 
	// returns 1 upon child opening
	int		OpenChildWidget(WidgetType &type);

	// will set the clicked text box index so that it can be used for adding text to the text box, updating textbox (render)
	void	StartTextInput();
	// will set the index to -1, meaning no text box is recieving input
	void	StopTextInput();

	// returns true if there's a text box recieving text input, otherwise false
	bool	IsRecievingTextInput();

	// will set the clicked select box index so that it can be used for scrolling, updating selectBox (render)
	// returns false if there's no select box with set id, writes to log
	bool	SetActiveSelectBox();

	// will set the index to -1, meaning there's no select box recieving input
	void	UnsetActiveSelectBox();

	// appends text to an active text box
	// returns 1 upon appending text to the text box
	// returns 0 if the text hasn't been added because the limit has been reached
	// returns -1 if there's no active text box or if other things go wrong, writes to log the error
	int		AddTextToTextBox(const char *text);
	
	// returns false on error and writes to log
	bool	ClearActiveTextBoxText();

	// returns false on error and writes to log
	bool	DeleteLastLetterFromTextBox();

	_menuWrapper*	GetAllButtonMenu();

	// uses this function in a loop, each iteration will select a visible button menu if there are any
	// if not returns nullptr
	//std::vector<CButton>*	GetNextVisibleButtonMenu();
	_buttonWrapper*	GetButtons();

	// active text box - one that is recieving text input, so if there's a text update the render accesses the text box through this method
	// returns nullptr if there's no active text box
	CTextBox*	GetActiveTextBox();

	// returns the active select box, the one which has been last clicked (so that it can be scrolled, updated by render)
	// returns nullptr if there's no active select box
	CSelectBox* GetActiveSelectBox();

	// fills m_pWidgetsClean
	// calls CTextBox::VClear() -> clearing text ...
	// returns false on error, writes to log
	bool		CleanUpTextBox();

	// fills m_pWidgetsClean
	// calls CSelectBox::VClear() -> clearing text ...
	// returns false on error, writes to log
	bool		CleanUpSelectBox();

	// updates m_pWidgetsClean with the active text box, doesn't reset active text box index, id and it doesn't hide the text box (there's no VReset or VClear call)
	// used so that the render can update the texture
	void		SetTextBoxDestroyWidgetData();

	// updates m_pWidgetsClean with the active select box, doesn't reset active select box index, id and it doesn't hide the select box (there's no VReset or VClear call)
	// used so that the render can update the texture
	void		SetSelectBoxDestroyWidgetData();

	// used for checking which widgets to destroy
	_GUITypeIdWrapper*	GetDestroyWidgetData();
	void				ClearDestroyWidgetData();

	// returns the child widget type, meaning: returns the WidgetType of the newly created widget
	WidgetType	ChildWidgetType();

	// inserts a new item to a select box for selecting a map on load 
	// map will only be inserted if there's no such map already present
	// return false on error and writes to log
	bool				AddMapToAllMaps(const char* mapName);
};
