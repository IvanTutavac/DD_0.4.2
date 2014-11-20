#include "CMenu.h"
#include "StructsGUI.h"

CMenu::CMenu(MenuPosition pos, int parentX, int parentY, int buttonW, int buttonH, bool visible_)
{
	m_position = pos;
	m_visible = visible_;
	m_defaultVisible = visible_;

	m_parentX = parentX;
	m_parentY = parentY;
	m_buttonW = buttonW;
	m_buttonH = buttonH;

	m_type = WidgetType::menu;
}

CMenu::~CMenu()
{

}

void	CMenu::VClean()
{
	CWidget::VClean();

	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		m_vButtons[i].VClean();
	}
}

_widgetResponse	CMenu::OnMouseLeftPress(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		if (m_vButtons[i].IsSetLeftPress())
		{
			response = m_vButtons[i].OnMouseLeftPress(mouseX, mouseY);

			if (response.pressAction.size() > 0)
			{
				return	response;
			}
		}
	}

	return	response;
}

_widgetResponse	CMenu::OnMouseLeftRelease(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		if (m_vButtons[i].IsSetLeftRelease())
		{
			response = m_vButtons[i].OnMouseLeftRelease(mouseX, mouseY);

			if (response.pressAction.size() > 0)
			{
				return	response;
			}
		}
	}

	return	response;
}

_widgetResponse	CMenu::OnMouseRightPress(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		if (m_vButtons[i].IsSetRightPress())
		{
			response = m_vButtons[i].OnMouseRightPress(mouseX, mouseY);

			if (response.pressAction.size() > 0)
			{
				return	response;
			}
		}
	}

	return	response;
}

_widgetResponse	CMenu::OnMouseRightRelease(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		if (m_vButtons[i].IsSetRightRelease())
		{
			response = m_vButtons[i].OnMouseRightRelease(mouseX, mouseY);

			if (response.pressAction.size() > 0)
			{
				return	response;
			}
		}
	}

	return	response;
}

void	CMenu::InsertButton(const CButton &button)
{
	m_vButtons.push_back(button);

	m_vButtons.back().SetParentID(m_id);
}

void	CMenu::CalculateMenuSize()
{
	int endX = 0, endY = 0;

	if (m_position == MenuPosition::MenuPos_horizRight)
	{
		m_x = m_parentX + m_buttonW;
		m_y = m_parentY;
		endX = m_x, endY = m_y;

		for (size_t i = 0; i < m_vButtons.size(); i++)
		{
			m_vButtons[i].SetPosition(endX,endY,m_buttonW,m_buttonH);
			endX += m_buttonW;
		}

		m_w = endX;
		m_h = endY + m_buttonH;
	}
	else if (m_position == MenuPosition::MenuPos_horizLeft)
	{
		m_x = m_parentX;
		m_y = m_parentY;
		endX = m_x, endY = m_y;

		for (size_t i = 0; i < m_vButtons.size(); i++)
		{
			endX -= m_buttonW;
			m_vButtons[i].SetPosition(endX, endY, m_buttonW, m_buttonH);
		}

		m_w = endX;
		m_h = endY + m_buttonH;
	}
	else if (m_position == MenuPosition::MenuPos_vertDown)
	{
		m_x = m_parentX;
		m_y = m_parentY + m_buttonH;
		endX = m_x, endY = m_y;

		for (size_t i = 0; i < m_vButtons.size(); i++)
		{
			m_vButtons[i].SetPosition(endX, endY, m_buttonW, m_buttonH);
			endY += m_buttonH;
		}

		m_w = endX + m_buttonW;
		m_h = endY;
	}
	else if (m_position == MenuPosition::MenuPos_vertUp) // tested
	{
		m_x = m_parentX;
		m_h = m_parentY;
		endX = m_x, endY = m_h;

		for (size_t i = 0; i < m_vButtons.size(); i++)
		{
			endY -= m_buttonH;
			m_vButtons[i].SetPosition(endX, endY, m_buttonW, m_buttonH);
		}

		m_w = endX + m_buttonW - m_x;
		m_y = endY - m_y;
	}
}

bool	CMenu::IsShown()
{
	if (m_visible == true)
		return	true;
	return	false;
}

void	CMenu::SetShown()
{
	m_visible = true;
}

bool	CMenu::IsHidden()
{
	if (m_visible == false)
		return	true;
	return	false;
}

void	CMenu::SetHidden()
{
	m_visible = false;
}

std::vector<CButton>*	CMenu::GetButtons()
{
	return	&m_vButtons;
}

CButton*	CMenu::GetButton(const int index)
{
	if (index >= (int)m_vButtons.size() || index < 0)
		return	nullptr;

	return	&m_vButtons[index];
}

int	CMenu::GetButtonsSize()
{
	return	m_vButtons.size();
}

bool		CMenu::MouseWithinMenu(const int mouseX, const int mouseY) const
{
	return	MouseWithinWidget(mouseX, mouseY);
}

void		CMenu::VClear()
{
	CWidget::VClear();
}

void		CMenu::VReset()
{
	VClear();
	CWidget::VReset();
}

void		CMenu::Update(_GUITypeIdWrapper *value_)
{
	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		_GUITypeId t{};
		
		t.id = m_vButtons[i].GetWidgetID();
		t.type = WidgetType::button;

		value_->GUITypeId.push_back(t);
	}
}

bool	CMenu::IsParent(const int childID) 
{
	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		if (m_vButtons[i].GetChildID() == childID)
		{
			return	true;
		}
	}

	return	false;
}

int		CMenu::GetButtonIndex(const int widgetID)
{
	for (size_t i = 0; i < m_vButtons.size(); i++)
	{
		if (m_vButtons[i].GetWidgetID() == widgetID)
		{
			return	i;
		}
	}

	return	-1;
}
