#pragma once

#include "CWidget.h"
#include <string>
#include <vector>

class CTextBox : public CWidget
{
	char	*m_label;

	std::string		m_text;
	size_t			m_textLimit; // initial value is zero, for that vale we do not make a text limit check
	
	int				m_r1, m_g1, m_b1; // box color
	int				m_r2, m_g2, m_b2; // text color

	std::vector<WidgetAction>	m_actionsOnEnterPressed;
	std::vector<WidgetAction>	m_actionsOnEnterReleased;

public:

	// label -> explanation to the user what he/she should write to the textbox
	// pass nullptr if you do not want a label
	// r1, g1,b1 - box color r2,g2,b2 - text color
	CTextBox(char *label, int x_, int y_, int w_, int h_, int r1_, int g1_, int b1_, int r2_, int g2_, int b2_,size_t textLimit, bool visible_ = false);
	~CTextBox();

	void	VClean() override;

	void	SetOnEnterPressed(const WidgetAction &action);
	void	SetOnEnterReleased(const WidgetAction &action);
	void	SetOnEnterReleased(const std::initializer_list<const WidgetAction> &actions);

	// id of the active text box, to know which text box is the one recieving input
	_widgetResponse	OnEnterReleased(const int id);
	// id of the active text box, to know which text box is the one recieving input
	_widgetResponse	OnEnterPressed(const int id);

	// appends text_ to m_text
	// if text limit has been set, and if it the limit has been reached size > limit returns false
	// this information can be used to create warnings ....
	// the function will remove the characters that exceed the limit
	bool	AddText(const char *text_);
	// removes last character from m_text if any
	void	DeleteLastChar();

	void	ClearText();

	int		GetR1();
	int		GetG1();
	int		GetB1();

	int		GetR2();
	int		GetG2();
	int		GetB2();

	std::string	GetText();
	char*	GetLabel();

	// reset text, set active to false calls parent VClear()
	void	VClear() override;
	// reset state to default, calls VClear 
	void	VReset() override;
};
