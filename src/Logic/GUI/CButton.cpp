#include "CButton.h"
#include "EnumGui.h"
#include "../../Tools/StringWrapper.h"

CButton::CButton(const char *text, const int x, const int y, const int w, const int h,bool visible_)
{
	m_time = 0;
	m_timeReset = 0;
	m_hoverEventFired = false;
	m_hoverEventTimerStarted = false;
	m_visible = visible_;
	m_defaultVisible = visible_;

	DD_Strcpy(m_text, sizeof(m_text), text);
	m_x = x, m_y = y, m_h = h, m_w = w;

	m_type = WidgetType::button;
}

CButton::CButton(const char *text)
{
	m_time = 0;
	m_timeReset = 0;
	m_hoverEventFired = false;
	m_hoverEventTimerStarted = false;
	m_visible = false;
	m_defaultVisible = false;

	DD_Strcpy(m_text, sizeof(m_text), text);

	m_type = WidgetType::menu;
}

CButton::~CButton()
{

}

void	CButton::VClean()
{
	CWidget::VClean();
}

void	CButton::SetText(const char *text)
{
	DD_Strcpy(m_text, sizeof(m_text), text);
}

const char*	CButton::GetText()
{
	return	m_text;
}

void	CButton::SetPosition(const int x, const int y, const int w, const int h)
{
	m_x = x, m_y = y, m_w = w, m_h = h;
}

void		CButton::VClear()
{
	CWidget::VClear();
}

void		CButton::VReset()
{
	VClear();
	CWidget::VReset();
}
