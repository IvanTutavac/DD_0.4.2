#pragma once

#include "CWidget.h"
#include <vector>
#include <string>

enum class FontSize;

class CSelectBox : public CWidget
{
private:

	std::vector<std::string>	m_vItems;

	std::vector<WidgetAction>	m_vWheelActions;

	int		m_selectedItemIndex; // set by ProcessInput which is called when an event happens (OnMouse...), -1 if there's no item selected

	int		m_currentItemStart; // starting position of items being shown
	int		m_maxItemN; // number of items shown

	int				m_r1, m_g1, m_b1; // box color
	int				m_r2, m_g2, m_b2; // text color

	int				m_itemStartY; // m_y for the first item inside select box
	int				m_itemSizeY; // m_y2 = m_y1 + m_itemSizeY -> meaning, to get each new item m_y position, add m_itemSizeY to it

	FontSize		m_fontSize;

	// if a click happend on an item, sets the index of that item and returns true
	bool	ProcessInput(const int mouseX, const int mouseY);

public:

	// maxItemN -> maximum number of items which can be shown
	// visible_ -> false -> hidden (default), true -> visible
	CSelectBox(const int x_, const int y_, const int w_, const int h_, const int maxItemN, int r1_, int g1_, int b1_, int r2_, int g2_, int b2_, const FontSize &fontSize_, const bool visible_ = false);
	~CSelectBox();

	void	AddItem(const std::string &itemName);
	void	AddItem(const std::initializer_list<const char * const> &itemNames);
	
	bool	ItemExists(const std::string &itemName);

	void	VClean() override;

	// if a click happened on selectbox but not on an item returns _widgetReponse()
	
	_widgetResponse	OnMouseLeftPress(const int mouseX, const int mouseY) override;
	_widgetResponse	OnMouseLeftRelease(const int mouseX, const int mouseY) override;
	_widgetResponse	OnMouseRightPress(const int mouseX, const int mouseY) override;
	_widgetResponse	OnMouseRightRelease(const int mouseX, const int mouseY) override;

	// updates the currentItemStart
	// returns _widgetResponse which will have id, type and wheelAction set (render needs to know which selectBox to update and id and type will be used for getting the right one from CWindow)
	// mouseX1, mouseY1 -> mouse position. used for checking whether event happened inside the select box
	_widgetResponse OnMouseWheel(const int wheelY, const int mouseX1, const int mouseY1);

	void	SetOnMouseWheel(const WidgetAction &action);

	// will return empty string if there's no selected item or the index was wrong
	std::string		GetSelectedItem();

	// returns items which are currently visible (for render so that it can create a texture)
	// returns std::vector<std::string>() upon error and writes to log
	std::vector<std::string>	GetVisibleItems();

	int		GetR1();
	int		GetG1();
	int		GetB1();

	int		GetR2();
	int		GetG2();
	int		GetB2();

	int		GetItemSizeY();
	int		GetItemStartY();

	void	SetFontSize(const FontSize &fontSize_);
	FontSize	GetFontSize();

	void	VReset() override;
	void	VClear() override;
};
