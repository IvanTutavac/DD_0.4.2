#include "CTextBox.h"
#include "../../Tools/StringWrapper.h"
#include "../../Tools/debug.h"
#include "StructsGUI.h"

CTextBox::CTextBox(char *label_, int x_, int y_, int w_, int h_, int r1_, int g1_, int b1_, int r2_, int g2_, int b2_, size_t textLimit, bool visible_)
{
	m_label = nullptr;
	m_textLimit = textLimit;
	m_x = x_, m_y = y_, m_h = h_, m_w = w_;
	m_r1 = r1_, m_g1 = g1_, m_b1 = b1_;
	m_r2 = r2_, m_g2 = g2_, m_b2 = b2_;

	m_visible = visible_, m_defaultVisible = visible_;

	if (label_ != nullptr)
	{
		size_t s_ = std::char_traits<char>::length(label_);
		s_ += 1; // \0

		m_label = DD_NEW char[s_];

		DD_Strcpy(m_label, s_, label_);
	}

	m_type = WidgetType::textBox;
}

CTextBox::~CTextBox()
{
	//DD_DELETE_ARRAY(m_label);
}

void	CTextBox::VClean()
{
	CWidget::VClean();
	DD_DELETE_ARRAY(m_label);
}

void	CTextBox::SetOnEnterPressed(const WidgetAction &action)
{
	m_actionsOnEnterPressed.push_back(action);
}

void	CTextBox::SetOnEnterReleased(const WidgetAction &action)
{
	m_actionsOnEnterReleased.push_back(action);
}

void	CTextBox::SetOnEnterReleased(const std::initializer_list<const WidgetAction> &actions)
{
	for (auto &v : actions)
	{
		SetOnEnterPressed(v);
	}
}

_widgetResponse	CTextBox::OnEnterPressed(const int id)
{
	_widgetResponse response;

	if (id == m_id)
	{
		response.id = m_id;
		response.type = m_type;
		response.childID = m_childID;
		response.childType = m_childType;

		response.AddPressAction(m_actionsOnEnterPressed);
	}

	return	response;
}

_widgetResponse	CTextBox::OnEnterReleased(const int id)
{
	_widgetResponse response;

	if (id == m_id)
	{
		response.id = m_id;
		response.type = m_type;
		response.childID = m_childID;
		response.childType = m_childType;

		response.AddPressAction(m_actionsOnEnterReleased);
	}

	return	response;
}

bool	CTextBox::AddText(const char *text_)
{
	std::string str_{ text_ };

	m_text += str_;

	if (m_textLimit > 0 && m_text.size() > m_textLimit)
	{
		m_text = m_text.substr(0, m_textLimit);
		return	false;
	}

	return	true;
}

void	CTextBox::ClearText()
{
	m_text.clear();
}

void	CTextBox::DeleteLastChar()
{
	if (m_text.length() > 0)
	{
		m_text.pop_back();
	}
}

int	CTextBox::GetR1()
{
	return	m_r1;
}

int	CTextBox::GetG1()
{
	return	m_g1;
}

int	CTextBox::GetB1()
{
	return	m_b1;
}

int	CTextBox::GetR2()
{
	return	m_r2;
}

int	CTextBox::GetG2()
{
	return	m_g2;
}

int	CTextBox::GetB2()
{
	return	m_b2;
}

std::string	CTextBox::GetText()
{
	return	m_text;
}

char*	CTextBox::GetLabel()
{
	return	m_label;
}

void	CTextBox::VClear()
{
	CWidget::VClear();
	ClearText();
}

void	CTextBox::VReset()
{
	VClear();
	CWidget::VReset();
}
