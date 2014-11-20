#include "CSelectBox.h"
#include "StructsGUI.h"
#include "../../Tools/Log.h"

CSelectBox::CSelectBox(const int x_, const int y_, const int w_, const int h_, const int maxItemN, int r1_, int g1_, int b1_, int r2_, int g2_, int b2_, const FontSize &fontSize_, const bool visible_)
{
	m_x = x_, m_y = y_, m_w = w_, m_h = h_;

	m_r1 = r1_, m_g1 = g1_, m_b1 = b1_;
	m_r2 = r2_, m_g2 = g2_, m_b2 = b2_;

	SetFontSize(fontSize_);

	m_visible = visible_;
	m_maxItemN = maxItemN;
	m_currentItemStart = 0;
	m_selectedItemIndex = -1;

	m_itemSizeY = m_h / m_maxItemN;

	m_itemStartY = m_itemSizeY/4;

	m_type = WidgetType::selectBox;
}

CSelectBox::~CSelectBox()
{

}

void	CSelectBox::VClean()
{
	CWidget::VClean();
}

void	CSelectBox::AddItem(const std::string &itemName)
{
	m_vItems.push_back(itemName);
}

void	CSelectBox::AddItem(const std::initializer_list<const char * const> &itemNames)
{
	for (auto &tmp : itemNames)
	{
		AddItem(tmp);
	}
}

bool	CSelectBox::ItemExists(const std::string &itemName)
{
	for (size_t i = 0; i < m_vItems.size(); i++)
	{
		if (m_vItems[i] == itemName)
			return	true;
	}

	return	false;
}

void	CSelectBox::SetOnMouseWheel(const WidgetAction &action)
{
	m_vWheelActions.push_back(action);
}

_widgetResponse	CSelectBox::OnMouseLeftPress(const int mouseX, const int mouseY)
{
	if (ProcessInput(mouseX, mouseY))
	{
		return	CWidget::OnMouseLeftPress(mouseX, mouseY);
	}

	return	_widgetResponse();
}

_widgetResponse	CSelectBox::OnMouseLeftRelease(const int mouseX, const int mouseY)
{
	if (ProcessInput(mouseX, mouseY))
	{
		return	CWidget::OnMouseLeftRelease(mouseX, mouseY);
	}

	return	_widgetResponse();
}

_widgetResponse	CSelectBox::OnMouseRightPress(const int mouseX, const int mouseY)
{
	if (ProcessInput(mouseX, mouseY))
	{
		return	CWidget::OnMouseRightPress(mouseX, mouseY);
	}

	return	_widgetResponse();
}

_widgetResponse	CSelectBox::OnMouseRightRelease(const int mouseX, const int mouseY)
{
	if (ProcessInput(mouseX, mouseY))
	{
		return	CWidget::OnMouseRightRelease(mouseX, mouseY);
	}

	return	_widgetResponse();
}

_widgetResponse CSelectBox::OnMouseWheel(const int wheelY,const int mouseX1, const int mouseY1)
{
	_widgetResponse response{};

	if (MouseWithinWidget(mouseX1, mouseY1))
	{
		m_currentItemStart += (-1 * wheelY);

		if (m_currentItemStart < 0)
		{
			m_currentItemStart = 0;
		}
		else if (m_currentItemStart >= static_cast<int>(m_vItems.size()))
		{
			m_currentItemStart = static_cast<int>(m_vItems.size() - 1);
		}

		response.AddWheelAction(m_vWheelActions);

		response.id = m_id;
		response.type = m_type;
	}

	return	response;
}

bool	CSelectBox::ProcessInput(const int mouseX, const int mouseY)
{
	if (MouseWithinWidget(mouseX, mouseY))
	{
		/*
			0 index
			1
			2
			400 y position
			3
			440 
			4
			480 
			5
			520 
			6
			560 
			7
			600 

			lets say click happened on 510... (int)((510 - 400)/40) = 2
			3 + 2 = 5
		*/

		int tmpIndex = (int)((mouseY-m_itemStartY - m_y)/m_itemSizeY);

		m_selectedItemIndex = tmpIndex + m_currentItemStart;

		if (m_selectedItemIndex >= (int)m_vItems.size())
		{
			return	false;
		}

		return	true;
	}
	
	return	false;
}

std::string	CSelectBox::GetSelectedItem()
{
	if (m_selectedItemIndex < 0 || m_selectedItemIndex >= (int)m_vItems.size())
		return	"";

	return	m_vItems[m_selectedItemIndex];
}

std::vector<std::string>	CSelectBox::GetVisibleItems()
{
	if (m_currentItemStart < 0 || (m_currentItemStart >= (int)m_vItems.size() && m_currentItemStart != 0))
	{
		Log("CSelectBox::GetVisibleItems failed, wrong m_currentItemStart");

		return	std::vector<std::string>();
	}

	std::vector<std::string> response;

	size_t i = m_currentItemStart;

	for (int j = 0; i < m_vItems.size() && j < m_maxItemN; i++, j++) // j limits the number of items shown
	{																 // i limits to the array size
		response.push_back(m_vItems[i]);
	}

	return	response;
}

void	CSelectBox::VClear()
{
	CWidget::VClear();
	m_currentItemStart = 0;
	m_selectedItemIndex = -1;
}

void	CSelectBox::VReset()
{
	VClear();
	CWidget::VReset();
}

int	CSelectBox::GetR1()
{
	return	m_r1;
}

int	CSelectBox::GetG1()
{
	return	m_g1;
}

int	CSelectBox::GetB1()
{
	return	m_b1;
}

int	CSelectBox::GetR2()
{
	return	m_r2;
}

int	CSelectBox::GetG2()
{
	return	m_g2;
}

int	CSelectBox::GetB2()
{
	return	m_b2;
}

FontSize CSelectBox::GetFontSize()
{
	return	m_fontSize;
}

void	CSelectBox::SetFontSize(const FontSize &fontSize_)
{
	m_fontSize = fontSize_;
}

int		CSelectBox::GetItemSizeY()
{
	return	m_itemSizeY;
}

int		CSelectBox::GetItemStartY()
{
	return	m_itemStartY;
}
