#pragma once

#include "CButton.h"
#include <vector>

enum class MenuPosition
{
	MenuPos_nothing,
	MenuPos_horizLeft,
	MenuPos_horizRight,
	MenuPos_vertUp,
	MenuPos_vertDown
};

enum class MouseEvents;
enum class InputState;

struct _GUITypeIdWrapper;

class CMenu : public CWidget
{
	std::vector<CButton>	m_vButtons;

	MenuPosition			m_position;
	
	int				m_parentX;
	int				m_parentY;
	int				m_buttonW;
	int				m_buttonH;

public:

	CMenu() = delete;
	// parentX, parentY -> parent button x and y positions
	// default visibility is false ( the menu is hidden )
	CMenu(MenuPosition pos, int parentX, int parentY, int buttonW, int buttonH, bool visible_ = false);
	~CMenu();

public:

	// must be called !
	// sets x,y,w,h of menu and buttons
	// renderer can then make a texture of such size
	void			CalculateMenuSize();

	_widgetResponse	OnMouseLeftPress(const int mouseX, const int mouseY) override;
	_widgetResponse	OnMouseLeftRelease(const int mouseX, const int mouseY) override;
	_widgetResponse	OnMouseRightPress(const int mouseX, const int mouseY) override;
	_widgetResponse	OnMouseRightRelease(const int mouseX, const int mouseY) override;

	// Insert a button inside the menu, the button will also have its parentID value updated
	void			InsertButton(const CButton &button);

	void	VClean() override;

	bool			IsHidden();
	void			SetHidden();

	bool			IsShown();
	void			SetShown();

	// check if a movement
	bool		MouseWithinMenu(const int mouseX, const int mouseY) const;

	std::vector<CButton>*	GetButtons();

	// returns nullptr if there's no such button
	CButton*		GetButton(const int index);

	// calls m_vButtons.Size()
	int				GetButtonsSize();

	// reset text, calls parent VClear()
	void			VClear() override;
	// reset state to default, calls VClear 
	void			VReset() override;

	// update argument with button id of each button inside the menu and set type to button
	// main use -> so that render can clean buttons
	void			Update(_GUITypeIdWrapper *value_);

	// checks whether a widget is a child of a button inside the menu
	// returns true if yes otherwise false
	bool			IsParent(const	int	childID);

	// returns the index of the button inside the menu
	// returns -1 if there's no such button widget
	int				GetButtonIndex(const int widgetID);
};
