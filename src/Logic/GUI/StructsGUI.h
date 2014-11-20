#pragma once

#include <vector>

#include "CTextBox.h"
#include "CMenu.h"
#include "CSelectBox.h"
#include "EnumGui.h"

// just try to not include this file inside a header file
// used to not include gui files inside header but to use forward declarations of structs in here


struct _textBoxWrapper
{
	std::vector<CTextBox>	textBox;
};

struct _buttonWrapper
{
	std::vector<CButton>	button;
};

struct _menuWrapper
{
	std::vector<CMenu>	menu;
};

struct _selectBoxWrapper
{
	std::vector<CSelectBox> selectBox;
};

struct _GUITypeId
{
	int	id; 
	WidgetType type; 
};

struct _GUITypeIdWrapper
{
	std::vector<_GUITypeId> GUITypeId;
};

struct _widgetResponse
{
	std::vector<WidgetAction> pressAction;
	std::vector<WidgetAction> movementAction;
	std::vector<WidgetAction> wheelAction;

	WidgetType	 childType; // used for knowing which widget we are dealing with
	WidgetType	 type; // will be menu for a button inside a menu 
	int	index; // probably menu index, after you redo menu class delete this, childID should be used for it !!!
	int	id; // parentID
	int	childID;
	
	bool waitForPress;

	_widgetResponse() :childType(WidgetType::nothing), index(-1), id(-1), childID(-1), waitForPress(false){}

	void	AddPressAction(const std::vector<WidgetAction> &action)
	{
		for (size_t i = 0; i < action.size(); i++)
		{
			pressAction.push_back(action[i]);
		}
	}

	void	AddMovement(const std::vector<WidgetAction> &action)
	{
		for (size_t i = 0; i < action.size(); i++)
		{
			movementAction.push_back(action[i]);
		}
	}

	void	AddWheelAction(const std::vector<WidgetAction> &action)
	{
		for (size_t i = 0; i < action.size(); i++)
		{
			wheelAction.push_back(action[i]);
		}
	}
};

struct _widgetActionData
{
	std::vector<WidgetAction>	onLeftPress;
	std::vector<WidgetAction>	onLeftRelease;
	std::vector<WidgetAction>	onRightPress;
	std::vector<WidgetAction>	onRightRelease;
	std::vector<WidgetAction>	onMouseEntering;
	std::vector<WidgetAction>	onMouseLeaving;
};