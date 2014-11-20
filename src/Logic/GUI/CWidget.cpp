#include "CWidget.h"
#include "StructsGUI.h"
#include "../../Data/Enums.h"
#include "../../Data/Globals.h"
#include "../../Tools/StringWrapper.h"
#include "../../Tools/debug.h"

CWidget::CWidget()
{
	m_childType = WidgetType::nothing;
	m_type = WidgetType::nothing;
	m_h = 0, m_w = 0, m_x = 0, m_y = 0;
	m_time = 0, m_timeReset = 0;
	m_id = CGlobal::GetWidgetID();
	m_parentID = -1;
	m_childID = -1;
	m_openedChildID = -1;
	m_name = nullptr;
	m_pAction = nullptr;
	m_pAction = DD_NEW _widgetActionData();
}

CWidget::~CWidget()
{
	//DD_DELETE_ARRAY(m_name);
}

void	CWidget::VClean()
{
	DD_DELETE(m_pAction);
	DD_DELETE_ARRAY(m_name);
}

int	CWidget::GetX()
{
	return	m_x;
}

int	CWidget::GetY()
{
	return	m_y;
}

int	CWidget::GetW()
{
	return	m_w;
}

int	CWidget::GetH()
{
	return	m_h;
}

_widgetResponse	CWidget::OnMouseHover(const int mouseX, const int mouseY, const double deltaTime)
{
	// usually used for showing new buttons, tooltip, item info, spell info or something else when hovering over a GUI element
	// for every show event, put on leaving clean event

	_widgetResponse response{};
	response.movementAction.push_back(WidgetAction::nothing); 
	response.pressAction.push_back(WidgetAction::nothing);
	response.index = -1;

	if (m_pAction->onMouseEntering[0] == WidgetAction::nothing)
	{
		return response;
	}

	if (MouseWithinWidget(mouseX, mouseY) == true) // mouse inside 
	{
		m_hoverEventTimerStarted = true; // start updating the timer, after it reaches 0 return	the event
	}
	else // mouse outside of the button
	{
		if (m_hoverEventFired == true) // has the event already been fired 
		{
			m_hoverEventFired = false;
			response.movementAction[0] = m_pAction->onMouseLeaving[0];
			return	response; // usually some clean up event 
		}
	}

	if (m_hoverEventTimerStarted == true)
	{
		m_time -= deltaTime; // update the time

		if (m_time < 0) 
		{	// enough time has passed upon entering the button, return the action
			m_time = m_timeReset; // reset the timer
			m_hoverEventFired = true;
			m_hoverEventTimerStarted = false; // we do not need to update the timer anymore
			
			response.movementAction[0] = m_pAction->onMouseEntering[0];
			return	response;
		}
	}

	return	response;
}

void	CWidget::UpdateWidgetOnMouseResponse(_widgetResponse &response)
{
	response.id = m_id;
	response.childID = m_childID;
	response.childType = m_childType;
	response.type = m_type;
}

_widgetResponse CWidget::OnMouseLeftPress(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	if (MouseWithinWidget(mouseX, mouseY) == true)
	{
		response.AddPressAction(m_pAction->onLeftPress);
		UpdateWidgetOnMouseResponse(response);

		for (size_t i = 0; i < m_pAction->onLeftPress.size(); i++)
		{
			if (m_pAction->onLeftPress[i] == WidgetAction::wait)
			{
				response.waitForPress = true;
				break;
			}
			else
			{
				response.waitForPress = false;
			}
		}
	}

	return	response;
}

_widgetResponse	CWidget::OnMouseLeftRelease(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	if (MouseWithinWidget(mouseX, mouseY) == true)
	{
		response.AddPressAction(m_pAction->onLeftRelease);
		UpdateWidgetOnMouseResponse(response);

		for (size_t i = 0; i < m_pAction->onLeftRelease.size(); i++)
		{
			if (m_pAction->onLeftRelease[i] == WidgetAction::wait)
			{
				response.waitForPress = true;
				break;
			}
			else
			{
				response.waitForPress = false;
			}
		}
	}

	return	response;
}

_widgetResponse	CWidget::OnMouseRightPress(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	if (MouseWithinWidget(mouseX, mouseY) == true)
	{
		response.AddPressAction(m_pAction->onRightPress);
		UpdateWidgetOnMouseResponse(response);

		for (size_t i = 0; i < m_pAction->onRightPress.size(); i++)
		{
			if (m_pAction->onRightPress[i] == WidgetAction::wait)
			{
				response.waitForPress = true;
				break;
			}
			else
			{
				response.waitForPress = false;
			}
		}
	}

	return	response;
}

_widgetResponse	CWidget::OnMouseRightRelease(const int mouseX, const int mouseY)
{
	_widgetResponse response{};

	if (MouseWithinWidget(mouseX, mouseY) == true)
	{
		response.AddPressAction(m_pAction->onRightRelease);
		UpdateWidgetOnMouseResponse(response);

		for (size_t i = 0; i < m_pAction->onRightRelease.size(); i++)
		{
			if (m_pAction->onRightRelease[i] == WidgetAction::wait)
			{
				response.waitForPress = true;
				break;
			}
			else
			{
				response.waitForPress = false;
			}
		}
	}

	return	response;
}

void	CWidget::SetOnLeftPress(const WidgetAction &a)
{
	m_pAction->onLeftPress.push_back(a);
}

void	CWidget::SetOnLeftRelease(const WidgetAction &a, bool pressNeeded)
{
	m_pAction->onLeftRelease.push_back(a);

	if (pressNeeded == true)
	{
		m_pAction->onLeftPress.push_back(WidgetAction::wait);
	}
}

void	CWidget::SetOnLeftRelease(const std::initializer_list<std::pair<const WidgetAction &, bool>> &values)
{
	for (auto &v : values)
	{
		SetOnLeftRelease(v.first, v.second);
	}
}

void	CWidget::SetOnRightPress(const WidgetAction &a)
{
	m_pAction->onRightPress.push_back(a);
}

void	CWidget::SetOnRightRelease(const WidgetAction &a, bool pressNeeded)
{
	m_pAction->onRightRelease.push_back(a);

	if (pressNeeded == true)
	{
		m_pAction->onRightPress.push_back(WidgetAction::wait);
	}
}

bool	CWidget::IsSetLeftPress()
{
	if (m_pAction->onLeftPress.size() < 1)
		return	false;

	return	true;
}

bool	CWidget::IsSetLeftRelease()
{
	if (m_pAction->onLeftRelease.size() < 1)
		return	false;

	return	true;
}

bool	CWidget::IsSetRightPress()
{
	if (m_pAction->onRightPress.size() < 1)
		return	false;

	return	true;
}

bool	CWidget::IsSetRightRelease()
{
	if (m_pAction->onRightRelease.size() < 1)
		return	false;

	return	true;
}

bool	CWidget::IsSetHover()
{
	if (m_pAction->onMouseEntering.size() < 1)
		return	false;

	return	true;
}

void	CWidget::SetOnMouseHover(const WidgetAction &OnEntering, const WidgetAction &OnLeaving, const double t)
{
	m_pAction->onMouseEntering.push_back(OnEntering);
	m_pAction->onMouseLeaving.push_back(OnLeaving);
	m_timeReset = t;
	m_time = m_timeReset;
}

bool	CWidget::MouseWithinWidget(const int mouseX, const int mouseY) const
{
	if (mouseX >= m_x && mouseX <= m_x + m_w && mouseY >= m_y && mouseY < m_y + m_h)
		return	true;

	return	false;
}

int		CWidget::GetWidgetID()
{
	return	m_id;
}

void	CWidget::SetChildID(const int value_)
{
	m_childID = value_;
}

int		CWidget::GetChildID()
{
	return	m_childID;
}

void	CWidget::SetName(const char *name_)
{
	size_t s_ = std::char_traits<char>::length(name_) + 1;
	
	m_name = DD_NEW char[s_];

	DD_Strcpy(m_name, s_, name_);
}

char*	CWidget::GetName()
{
	return	m_name;
}

int		CWidget::CompareName(const char *name_)
{
	if (name_ == nullptr || m_name == nullptr)
		return	-1;

	if (std::char_traits<char>::compare(m_name, name_, std::char_traits<char>::length(m_name)) == 0)
		return	1;

	return	0;
}

void	CWidget::SetChildWidgetType(const WidgetType &value_)
{
	m_childType = value_;
}

WidgetType	CWidget::GetChildWidgetType()
{
	return	m_childType;
}

bool	CWidget::IsVisible()
{
	if (m_visible == true)
		return	true;
	return	false;
}

void	CWidget::SetHidden()
{
	m_visible = false;
}

void	CWidget::SetShown()
{
	m_visible = true;
}

bool	CWidget::IsDefaultVisible()
{
	if (m_defaultVisible == true)
		return	true;
	return	false;
}

void	CWidget::VClear()
{
	SetHidden();
}

void	CWidget::VReset()
{
	if (IsDefaultVisible()) // has the widget been visible from the start
	{
		SetShown();
	}
	else // widget has not been visible from the start
	{
		SetHidden();
	}
}

void	CWidget::SetOpenedChildID(const int value_)
{
	m_openedChildID = value_;
}

int		CWidget::GetOpenedChildID() const
{
	return	m_openedChildID;
}

void	CWidget::SetParentID(const int value_)
{
	m_parentID = value_;
}

int		CWidget::GetParentID() const
{
	return	m_parentID;
}
